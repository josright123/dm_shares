#include "dm9051opts.h"
#include "dm9051_lw.h"
#include "dm9051_lw_conf.h"
#include "dm9051_cboard.h"
#include "dm9051_lw_cspi.h"
#include "dm9051_lw_cint.h"
#include "dm9051_lw_log.h"
#include "dm9051_lw_debug.h"
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
//#include "dm9051_bl_xxx.h"

static void dm9051_delay_in_core_process(uint16_t nms, char *zhead) //finally, dm9051_lw.c
{
	if (nms)
		printf(": dm9051_driver[%d] ::: %s delay %u ms.. : \r\n", 0, zhead, nms);
	dm_delay_ms(nms);
}

uint16_t impl_read_chip_id(void) {
	uint8_t buff[2];
	cspi_read_regs(DM9051_PIDL, buff, 2, CS_EACH);
	return buff[0] | buff[1] << 8;
}

void read_chip_revision(uint8_t *ids, uint8_t *rev_ad) {
	cspi_read_regs(DM9051_VIDL, ids, 5, CS_EACH); //OPT_CS(csmode)
	cspi_read_regs(0x5C, rev_ad, 1, CS_EACH); //OPT_CS(csmode)
}

void impl_read_rx_pointers(uint16_t *rwpa_wt, uint16_t *mdra_rd) {
	*rwpa_wt = (uint16_t)cspi_read_reg(0x24) | (uint16_t)cspi_read_reg(0x25) << 8; //DM9051_RWPAL
	*mdra_rd = (uint16_t)cspi_read_reg(0x74) | (uint16_t)cspi_read_reg(0x75) << 8; //DM9051_MRRL;
}

void impl_read_par(uint8_t *buff) {
	cspi_read_regs(DM9051_PAR, buff, 6, CS_EACH);
}

uint16_t impl_phy_read(uint16_t uReg) {
	int w = 0;

	#if 1
	//CHMODE
	if (uReg == PHY_STATUS_REG)
		dm9051_phycore_on(0);
	#endif

	cspi_write_reg(DM9051_EPAR, DM9051_PHY | uReg);
	cspi_write_reg(DM9051_EPCR, 0xc);
	dm_delay_us(1);
	while(cspi_read_reg(DM9051_EPCR) & 0x1) {
		dm_delay_us(1);
		if (++w >= 500) //5
			break;
	} //Wait complete

	cspi_write_reg(DM9051_EPCR, 0x0);
	return (uint16_t)cspi_read_reg(DM9051_EPDRL) | (uint16_t)cspi_read_reg(DM9051_EPDRH) << 8;
}

void dm9051_reset_pulse(void)
{
	cpin_poweron_reset();
}

void dm9051_phycore_on(uint16_t nms) {
	  cspi_write_reg(DM9051_GPR, 0x00);		//Power on PHY
	  dm9051_delay_in_core_process(nms, "_phycore_on<>");
}

void dm9051_ncr_reset(uint16_t nms) {
	  cspi_write_reg(DM9051_NCR, DM9051_REG_RESET); //iow(DM9051_NCR, NCR_RST);
	  dm9051_delay_in_core_process(nms, "_core_reset<>"); //dm_delay_ms(250); //CH390-Est-Extra
}

void impl_dm9051_set_par(const uint8_t *macadd)
{
	int i;
	for (i=0; i<6; i++)
		cspi_write_reg(DM9051_PAR+i, macadd[i]);
}

void dm9051_set_mar(void)
{
	int i;
	for (i=0; i<8; i++)
		cspi_write_reg(DM9051_MAR+i, (i == 7) ? 0x80 : 0x00);
}

void dm9051_set_recv(void)
{
	#if 0
//	cspi_write_reg(_DM9051_FCR, _FCR_DEFAULT_CONF); Located in 'dm9051 core reset'!
//	phy_write _04, _flow
	#endif
	identify_irq_stat(ISTAT_DM_IMR);
	cspi_write_reg(DM9051_IMR, IMR_PAR | IMR_PRM); //(IMR_PAR | IMR_PTM | IMR_PRM);
	identify_irq_stat(ISTAT_DM_RCR);
	cspi_write_reg(DM9051_RCR, RCR_DEFAULT | RCR_RXEN);
}

void dm9051_soft_default(void)
{
	uint8_t val = MBNDRY_BYTE; //8-bit
//	val = MBNDRY_WORD; //this line, 16-bit, only 'if (!OPT_CONFIRM(onlybytemode) && (OPT_U8(iomode) == MBNDRY_WORD))'
	cspi_write_reg(DM9051_MBNDRY, val); //(MBNDRY_WORD, "16-bit mode")
	cspi_write_reg(DM9051_PPCR, PPCR_PAUSE_COUNT);
	cspi_write_reg(DM9051_LMCR, LMCR_MODE1);
	cspi_write_reg(DM9051_INTR, INTR_ACTIVE_LOW); //interrupt active low
}

uint16_t impl_dm9051_isr_disab(void) //static uint16_t dm9051_isr_clean(void)
{
	uint16_t isrs;

	identify_irq_stat(ISTAT_IRQ_NOW);
	isrs = cspi_read_reg(DM9051_ISR);
	return (isrs & ISR_PR) | (isrs << 8);
}

uint16_t impl_dm9051_isr_enab(void) //static uint16_t dm9051_isr_clean(void)
{
	uint16_t isrs;

	deidentify_irq_stat(ISTAT_IRQ_NOW);
	isrs = cspi_read_reg(DM9051_ISR);
	if (isrs & 1) {
		cspi_write_reg(DM9051_ISR, (uint8_t) isrs);
		isrs |= cspi_read_reg(DM9051_ISR) << 8;
	}
	return isrs;
}

void impl_disable_mcu_irq(void)
{
	cint_disable_mcu_irq();
}

void impl_enable_mcu_irq(void)
{
	cint_enable_mcu_irq();
}

#define DM9051_RX_BREAK(expression, handler) do { if ((expression)) { \
  handler;}} while(0)

uint16_t dm9051_rx_read(uint8_t *buff)
{
	uint16_t rx_len, pad = 0; //8-bit
	uint8_t rxbyte, rx_status;
	uint8_t ReceiveData[4];
	
	rxbyte = cspi_read_mem2x(); //DM9051_Read_Mem2X(); //DM9051_Read_Rxb();
	
	DM9051_RX_BREAK((rxbyte != 0x01 && rxbyte != 0), return ev_rxb(rxbyte));
	DM9051_RX_BREAK((rxbyte == 0), return 0);
		
	cspi_read_mem(ReceiveData, 4);
	cspi_write_reg(DM9051_ISR, 0x80);
	
	rx_status = ReceiveData[1];
	rx_len = ReceiveData[2] + (ReceiveData[3] << 8);
	DM9051_RX_BREAK((rx_status & 0xbf), return ev_status(rx_status));
	DM9051_RX_BREAK((rx_len > RX_POOL_BUFSIZE), return impl_dm9051_err_hdlr("_dm9051f[%d] rx_len error : %u\r\n", PINCOD, rx_len, 0));
//	pad = rx_len & 1; //this line, 16-bit, only 'if (!OPT_CONFIRM(onlybytemode) && (OPT_U8(iomode) == MBNDRY_WORD))'
	cspi_read_mem(buff, rx_len + pad);
	cspi_write_reg(DM9051_ISR, 0x80);

//if (rx_len >= 128) {
//	int titledn = 0;
//	sprint_hex_dump0(2, titledn, "dm9 head   <<rx", 4, 32, ReceiveData, 0, 4, DM_TRUE);
//}

//printf("_dm9051_rx buf %x, len %u ( -4 )\r\n", buff, rx_len);

//if (rx_len >= 128) {
//	int titledn = 0;
//	sprint_hex_dump0(2, titledn, "dm9 monitor   <<rx", rx_len, 32, buff, 0, (rx_len < 66) ? rx_len : 66, DM_TRUE); 
//}

	return rx_len;
}

#define DM9051_TX_DELAY(expression, handler) do { if ((expression)) { \
  handler;}} while(0)

void dm9051_tx_write(uint8_t *buf, uint16_t len)
{
	uint16_t pad = 0; //8-bit
	cspi_write_reg(DM9051_TXPLL, len & 0xff);
	cspi_write_reg(DM9051_TXPLH, (len >> 8) & 0xff);
//	pad = len & 1; //this line, 16-bit, only 'if (!OPT_CONFIRM(onlybytemode) && (OPT_U8(iomode) == MBNDRY_WORD))'
	cspi_write_mem(buf, len + pad);

//printf("dm9051_tx buf %x, len %u\r\n", buf, len);

//if (len >= 128) {
//	int titledn = 0;
//	sprint_hex_dump0(2, titledn, "dm9 monitor   tx>>", len, 32, buf, 0, (len < 66) ? len : len, DM_TRUE);
//}
}

void dm9051_tx_req(void)
{
	cspi_write_reg(DM9051_TCR, TCR_TXREQ); /* Cleared after TX complete */
	DM9051_TX_DELAY((cspi_read_reg(DM9051_TCR) & TCR_TXREQ), dm_delay_us(5));
}

//[dm9051_if.c]

//rxb.0
static void dm9051_show_rxbstatistic(uint8_t *htc, int n)
{
	int i, j;
	printf("SHW rxbStatistic, 254 wrngs\r\n");
	for (i = 0 ; i < (n+2); i++) {
		if (!(i%32) && i) printf("\r\n");
		if (!(i%32) || !(i%16)) printf("%02x:", i);
		if (!(i%8)) printf(" ");
		if (i==0 || i==1) {
			printf("  ");
			continue;
		}
		j = i - 2;
		printf("%d ", htc[j]);
	}
	printf("\r\n");
}
//rxb.1
#define	TIMES_TO_RST	10
static uint8_t ret_fire_time(uint8_t *histc, int csize, int i, uint8_t rxb)
{
	uint16_t rwpa_w, mdra_ingress;
	uint8_t times = (histc[i] >= TIMES_TO_RST) ? histc[i] : 0;

	impl_read_rx_pointers(&rwpa_w, &mdra_ingress);
	printf("%2d. rwpa %04x / igrss %04x, histc[rxb %02xh], times= %d\r\n",
		 histc[i], rwpa_w, mdra_ingress, rxb, times);

	if (times) {
		dm9051_show_rxbstatistic(histc, csize);
		histc[i] = 1;
	}
	return times;
}

//rxb
uint16_t ev_rxb(uint8_t rxb)
{
	unsigned int i;
	static uint8_t histc[254] = { 0 };
	uint8_t times = 1;

	for (i = 0 ; i < sizeof(histc); i++) {
		if (rxb == (i+2)) {
			histc[i]++;
			times = ret_fire_time(histc, sizeof(histc), i, rxb);
			return impl_dm9051_err_hdlr("_dm9051f[%d] : rxbErr %u times :: dm9051_core_reset()\r\n", 0, times, 1); //PINCOD= 0 //As: Hdlr (times : TIMES_TO_RST or 0)
		}
	}
	return impl_dm9051_err_hdlr(" _dm9051f[%d] rxb error times (No way!) : %u\r\n", 0, times, 0); //PINCOD= 0 //As: Hdlr (times : 1, zerochk : 0)
}

//status
uint16_t ev_status(uint8_t rx_status)
{
	bannerline_log();
	printf(".(Err.status%2x) _dm9051f:", rx_status);
	if (rx_status & RSR_RF) printf(" runt-frame");

	if (rx_status & RSR_LCS) printf(" late-collision");
	if (rx_status & RSR_RWTO) printf(" watchdog-timeout");
	if (rx_status & RSR_PLE) printf(" physical-layer-err");
	if (rx_status & RSR_AE) printf(" alignment-err");
	if (rx_status & RSR_CE) printf(" crc-err");
	if (rx_status & RSR_FOE) printf(" rx-memory-overflow-err");
	bannerline_log();
	return impl_dm9051_err_hdlr("_dm9051f[%d] rx_status error : 0x%02x\r\n", 0, rx_status, 0); //PINCOD= 0
}
