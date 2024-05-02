/*
 * Copyright (c) 2023-2025 Davicom Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This DM9051 Driver for LWIP tcp/ip stack
 * First veryification with : AT32F415
 *
 * Author: Joseph CHANG <joseph_chang@davicom.com.tw>
 * Date: 20230411
 * Date: 20230428 (V3)
 * Date: 20240215 (Versions log please refer to 'dm9051_lw/info.txt')
 */

/* SPI master control is essential,
 * Do define spi specific definition, depend on your CPU's board support package library.
 * Here we had the definition header file with : "at32f415_spi.h"
 * for AT32F415 cpu.
 */
#if 0 //only for Lwip
#include <lwip/sys.h> //.temp
#endif
//#include "lwip/tcpip.h" //.temp
#include "dm9051opts.h"
#include "dm9051_lw.h"
#include "dm9051_lw_debug.h"

// printf("(dm9 xfer) %.8x: %s", i, linebuf);
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG, ("lw.c: " fmt, ##__VA_ARGS__))

//static void dm9051_phycore_on(uint16_t nms);
static void dm9051_core_reset(void);
static void impl_dm9051_set_par(const uint8_t *macadd);
static void dm9051_set_mar(void);
static void dm9051_set_recv(void);

#if 0 //[Since lwip_rx_hdlr()/lwip_rx_loop_handler() can do check the link-state in advance, so this logic can then discard!]
static u8 lw_flag[ETHERNET_COUNT];
static u16 unlink_count[ETHERNET_COUNT], unlink_stamp[ETHERNET_COUNT];
#endif

//#define UNLINK_COUNT_RESET	60000

#if freeRTOS
sys_mutex_t lock_dm9051_core;
#endif

void dm9051_poweron_rst(void)
{
	DM9051_Poweron_Reset();
}

int check_chip_id(uint16_t id) {

	if (id == (0x91001c00 >> 16)) {
		printf("---------------------- \r\n");
		printf("--- warn case %04x --- \r\n", id);
		printf("---------------------- \r\n");
	}

	return (id == (DM9051_ID >> 16)
			|| id == (DM9052_ID >> 16)
	#if 1
	//_CH390
			|| id == (0x91511c00 >> 16)
			|| id == (0x91001c00 >> 16) /* Test CH390 */
	#endif
		) ? 1 : 0;
}

static void read_chip_revision(u8 *ids, u8 *rev_ad) {
	cspi_read_regs(DM9051_VIDL, ids, 5, OPT_CS(csmode)); //dm9051opts_csmode_tcsmode()
	cspi_read_regs(0x5C, rev_ad, 1, OPT_CS(csmode)); //dm9051opts_csmode_tcsmode()
}

//.#if TEST_PLAN_MODE
//.#define check_get()				1
//.#define check_get_check_done()	0
//.#define	check_decr_to_done()
//.#define	check_set_done()
//.#define	check_set_new()
//.#define tp_all_done()	1

//. static void rxrp_dump_print_init_show(void) {
//.	uint16_t rxrp;
//.	rxrp = cspi_read_reg(DM9051_MRRL);
//.	rxrp |= cspi_read_reg(DM9051_MRRH) << 8;
//.	printf(" %4x", rxrp);
//. }
//.#endif

uint16_t eeprom_read(uint16_t wordnum)
{
	u16 uData;
	do {
		int w = 0;
		DM9051_Write_Reg(DM9051_EPAR, wordnum);
		DM9051_Write_Reg(DM9051_EPCR, 0x4); // chip is reading
		dm_delay_us(1);
		while(DM9051_Read_Reg(DM9051_EPCR) & 0x1) {
			dm_delay_us(1);
			if (++w >= 500) //5
				break;
		} //Wait complete
		DM9051_Write_Reg(DM9051_EPCR, 0x0);
		uData = (DM9051_Read_Reg(DM9051_EPDRH) << 8) | DM9051_Read_Reg(DM9051_EPDRL);
	} while(0);
	return uData;
}

//static ; function "phy_write" was declared but never referenced.
void phy_write(uint16_t reg, uint16_t value)
{
	int w = 0;

	DM9051_Write_Reg(DM9051_EPAR, DM9051_PHY | reg);
	DM9051_Write_Reg(DM9051_EPDRL, (value & 0xff));
	DM9051_Write_Reg(DM9051_EPDRH, ((value >> 8) & 0xff));
	/* Issue phyxcer write command */
	DM9051_Write_Reg(DM9051_EPCR, 0xa);
	dm_delay_us(1);
	while(DM9051_Read_Reg(DM9051_EPCR) & 0x1){
		dm_delay_us(1);
		if (++w >= 500) //5
			break;
	} //Wait complete

	DM9051_Write_Reg(DM9051_EPCR, 0x0);
}

uint16_t dm9051_bmcr_update(void) {
  return impl_phy_read(PHY_CONTROL_REG);
}
uint16_t dm9051_link_update(void) {
  return impl_phy_read(PHY_STATUS_REG);
}
uint16_t dm9051_phy_read(uint32_t reg) {
  return impl_phy_read(reg);
}
void dm9051_phy_write(uint32_t reg, uint16_t value) {
  phy_write(reg, value);
}

uint16_t dm9051_eeprom_read(uint16_t word) {
  return eeprom_read(word);
}

void test_plan_mbndry(void)
{
	uint8_t isr0, isr1, mbndry0, mbndry1;
	char *str0, *str1;

	isr0 = DM9051_Read_Reg(DM9051_ISR);

	mbndry0 = OPT_U8(iomode);
	str0 = (mbndry0 & MBNDRY_BYTE) ? "(8-bit mode)" : "(16-bit mode)";
	printf("  ACTION: Start.s Write the MBNDRY %02x %s\r\n", mbndry0, str0);

	DM9051_Write_Reg(DM9051_MBNDRY, mbndry0);

		mbndry1 = DM9051_Read_Reg(DM9051_MBNDRY);
		str1 = (mbndry1 & MBNDRY_BYTE) ? "(8-bit mode)" : "(16-bit mode)";
		if ((mbndry0 & MBNDRY_BYTE) == (mbndry1 & MBNDRY_BYTE))
			printf("  ACTION: !And.e Read check MBNDRY %02x %s\r\n", mbndry1, str1);
		else
			printf("  ACTION: !But.e Read check MBNDRY %02x %s \"( read different )\"\r\n",
				mbndry1, str1); //"(read diff, be an elder revision chip bit7 can't write)"

	isr1 = DM9051_Read_Reg(DM9051_ISR);

	printf("  RESET: ISR.read.s %02x %s\r\n", isr0, isr0 & 0x80 ? "(8-bit mode)" : "(16-bit mode)");
	printf("  RESET: ISR.read.e %02x %s\r\n", isr1, isr1 & 0x80 ? "(8-bit mode)" : "(16-bit mode)");
}

/*void test_plan_100mf(ncrmode_t ncrmode, u8 first_log)
{
	//
	// explicity, phy write bmcr, 0x3300,
	// Since test mode may stop in 10M Half
	// We nee Full duplex for using a loopback terminator
	// for self xmit/receive in starting test item.
	// observ, AN did take some more time to link up state
	// compare to non AN.
	//
	//if (_dm9051opts_ncrforcemode()) {
	  #if 1
		//[dm9052]
		if (ncrmode == NCR_FORCE_100MF) {
			phy_write(27, 0xe000);
			phy_write(0, 0x2100);
			if (first_log) {
				uint16_t bmcr= impl_phy_read(0);
				printf("  RESET: REG27 write : %04x\r\n", 0xe000);
				printf("  RESET: BMCR write/read : %04x/%04x\r\n", 0x2100, bmcr);
				printf("  RESET: _core_reset [set link parameters, Force mode for 100M Full]\r\n");
			}
		}
		//[dm9051c]
		else if (ncrmode == NCR_AUTO_NEG) {
			phy_write(0, 0x3300);
			if (first_log) {
				uint16_t bmcr= impl_phy_read(0);
				printf("  RESET: BMCR write/read : %04x/%04x\r\n", 0x3300, bmcr);
				printf("  RESET: _core_reset [set link parameters, A.N. for 100M Full]\r\n");
			}
		}
	  #endif
	//}
}*/

/*void test_plan_rx_checksum_enable(void)
{
	DM9051_Write_Reg(DM9051_RCSSR, RCSSR_RCSEN);
}*/

static void dm9051_delay_in_core_process(uint16_t nms, char *zhead) //finally, dm9051_lw.c
{
	if (nms)
		printf(": dm9051_driver[%d] ::: %s delay %u ms.. : \r\n", mstep_get_net_index(), zhead, nms);
	if (nms)
	  dm_delay_ms(nms); //_delay_ms(nms); //from James' advice! to be determined with a reproduced test cases!!
}

//static
void dm9051_phycore_on(uint16_t nms) {
	  DM9051_Write_Reg(DM9051_GPR, 0x00);		//Power on PHY
	  dm9051_delay_in_core_process(nms, "_phycore_on<>");
}

static void dm9051_ncr_reset(uint16_t nms) {
	  DM9051_Write_Reg(DM9051_NCR, DM9051_REG_RESET); //iow(DM9051_NCR, NCR_RST);
	  dm9051_delay_in_core_process(nms, "_core_reset<>"); //dm_delay_ms(250); //CH390-Est-Extra
}

static void dm9051_core_reset(void)
{
	if (OPT_CONFIRM(generic_core_rst)){

		/*		{	DM_MACRO (enable_t, generic_core_rst) } v.s.
		 * 		{	optsex_t dm9051optsex {DM_TRUE, "Long_delay core rst",} }
		 */

		#if 0
		printf("-----------------------------------------------------------------------------------------\r\n");
		printf("--------------------- write a long delay type procedure ,for core reset -----------------\r\n");
		printf("-----------------------------------------------------------------------------------------\r\n");
		#endif

		//CH390
		  //printf("  _core_reset[%d]()\r\n", mstep_get_net_index());

		  dm9051_ncr_reset(OPT_U8(hdir_x2ms)*2);
		  dm9051_phycore_on(250);

		  DM9051_Write_Reg(DM9051_MBNDRY, OPT_U8(iomode));
		  DM9051_Write_Reg(DM9051_PPCR, PPCR_PAUSE_COUNT); //iow(DM9051_PPCR, PPCR_SETTING); //#define PPCR_SETTING 0x08
		  DM9051_Write_Reg(DM9051_LMCR, LMCR_MODE1);
		  DM9051_Write_Reg(DM9051_INTR, INTR_ACTIVE_LOW); // interrupt active low
	} else {
		  int i = mstep_get_net_index();
		  u8 if_log = first_log_get(i); //+first_log_clear(i);

		  //u8 gpcr = DM9051_Read_Reg(DM9051_GPCR);
		  //DM9051_Write_Reg(DM9051_GPCR, gpcr | 0x01); //GPC0(reserved), bit-0

		#if 0
		  dm9051_clear_flags(lw_flag, DM9051_FLAG_LINK_UP);
		  unlink_count = unlink_stamp = 0;
		#endif

	#if 0
		//DAVICOM
		  printf("  _core_reset[%d]()\r\n", i);
		  dm9051_phycore_on(5);

		  DM9051_Write_Reg(DM9051_NCR, DM9051_REG_RESET); //iow(DM9051_NCR, NCR_RST);
		  dm_delay_ms(0);
	#else
		//CH390
		  printf("  _core_reset[%d]()\r\n", i);
		  DM9051_Write_Reg(DM9051_NCR, DM9051_REG_RESET); //iow(DM9051_NCR, NCR_RST);
		  dm_delay_ms(2); //CH390-Est-Extra

		  dm9051_phycore_on(5);
	#endif

		  do {
			if (if_log) {
			  test_plan_mbndry();
			} else
			  DM9051_Write_Reg(DM9051_MBNDRY, OPT_U8(iomode));
		  } while(0);

		  DM9051_Write_Reg(DM9051_PPCR, PPCR_PAUSE_COUNT); //iow(DM9051_PPCR, PPCR_SETTING); //#define PPCR_SETTING 0x08
		  DM9051_Write_Reg(DM9051_LMCR, LMCR_MODE1);
		  #if 1
		  DM9051_Write_Reg(DM9051_INTR, INTR_ACTIVE_LOW); // interrupt active low
		  #endif

		#if TEST_PLAN_MODE
		if (OPT_CONFIRM(rxmode_checksum_offload))
			DM9051_Write_Reg(DM9051_RCSSR, RCSSR_RCSEN);
		#else
		/* if (isrxmode_checksum_offload())
				DM9051_Write_Reg(DM9051_RCSSR, RCSSR_RCSEN); */
		#endif

		/* flow_control_config_init */
		if (OPT_CONFIRM(flowcontrolmode)) {
		  #if TEST_PLAN_MODE == 0
			printf("  core_reset: %s, fcr value %02x\r\n", DM_GET_DESC(enable_t, flowcontrolmode), FCR_DEFAULT_CONF);
		  #endif
			DM9051_Write_Reg(DM9051_FCR, FCR_DEFAULT_CONF);
		}

	#if TEST_PLAN_MODE
		test_plan_100mf(OPT_BMCR(bmcrmode), if_log); //= dm9051opts_bmcrmode_tbmcrmode()
	#endif
	}
}

static void dm9051_show_rxbstatistic(u8 *htc, int n)
{
	int i, j;

	//dm9051_show_id();
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

static void dm9051_mac_adr(const uint8_t *macadd) {
	impl_dm9051_set_par(macadd);
	//show_par();
}

void dm9051_rx_mode(void)
{
	dm9051_set_mar();
	//show_mar();

	dm9051_set_recv();
}

/*void dm9051_rx_mode_subprocess(void) {
	u8 buff[8];
	cspi_read_regs(DM9051_MAR, buff, 8, CS_EACH);
	printf("dm9051_rx_mode_subprocess MAR %02x %02x %02x %02x  %02x %02x %02x %02x\r\n",
			buff[0], buff[1], buff[2], buff[3], buff[4], buff[5], buff[6], buff[7]);
	cspi_read_regs(DM9051_PAR, buff, 6, CS_EACH);
	printf("dm9051_rx_mode_subprocess PAR %02x %02x %02x %02x %02x %02x\r\n",
			buff[0], buff[1], buff[2], buff[3], buff[4], buff[5]);
	dm9051_set_recv();
}*/

static void impl_dm9051_set_par(const u8 *macadd)
{
	int i;
	for (i=0; i<6; i++)
		DM9051_Write_Reg(DM9051_PAR+i, macadd[i]);
}
static void dm9051_set_mar(void)
{
	int i;
	for (i=0; i<8; i++)
		DM9051_Write_Reg(DM9051_MAR+i, (i == 7) ? 0x80 : 0x00);
}

static void dm9051_set_recv(void)
{
	#if 0
	DM9051_Write_Reg(_DM9051_FCR, _FCR_DEFAULT_CONF); Located in 'dm9051 core reset'!
	phy_write _04, _flow
	#endif

	DM9051_Write_Reg(DM9051_IMR, IMR_PAR | IMR_PRM); //(IMR_PAR | IMR_PTM | IMR_PRM);

	//#if _TEST_PLAN_MODE //#else //#endif
	if (OPT_U8(promismode)) {
		printf("SETRECV: ::: test rx_promiscous (write_reg, 0x05, (1 << 2))\r\n");
		DM9051_Write_Reg(DM9051_RCR, RCR_DEFAULT | RCR_PRMSC | RCR_RXEN); //promiscous
	}
	else
		DM9051_Write_Reg(DM9051_RCR, RCR_DEFAULT | RCR_RXEN); //dm9051_fifo_RX_enable();
}

char *display_identity_bannerline_title = NULL;
char *display_mac_bannerline_defaultN = ": Display Bare device";
char *display_identity_bannerline_defaultN = ": Bare device";
char *display_identity_bannerline_default =  ": Read device";

int display_identity(char *spiname, uint16_t id, uint8_t *ids, uint8_t id_adv, uint16_t idin, char *tail) {
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))

#if 0
	static uint16_t psh_id1[ETHERNET_COUNT];
#endif
	static uint8_t psh_ids1[ETHERNET_COUNT][5], psh_id_adv1[ETHERNET_COUNT];
	
	DM_UNUSED_ARG(spiname);
	
	DM_UNUSED_ARG(id);
	if (ids) {
#if 0
		psh_id1[mstep_get_net_index()] = id;
#endif	
		memcpy(&psh_ids1[mstep_get_net_index()][0], ids, 5);
		psh_id_adv1[mstep_get_net_index()] = id_adv;
	} else {
//		 id = psh_id1[mstep_get_net_index()];
//		 memcpy(ids, &psh_ids1[mstep_get_net_index()][0], 5);
//		 id_adv = psh_id_adv1[mstep_get_net_index()];
	}

//display_ids(fstr, ids) + display_ida(fstr, id_adv)
	printf("%s[%d] ::: ids %02x %02x %02x %02x (%s) chip rev %02x, Chip ID %04x (CS_EACH_MODE)%s\r\n",
		display_identity_bannerline_title ? display_identity_bannerline_title : display_identity_bannerline_default,
		mstep_get_net_index(),
		psh_ids1[mstep_get_net_index()][0], psh_ids1[mstep_get_net_index()][1],
		psh_ids1[mstep_get_net_index()][2], psh_ids1[mstep_get_net_index()][3], 
		DM_GET_DESC(csmode_t, csmode), //dm9051opts_desccsmode()
		psh_id_adv1[mstep_get_net_index()], 
		idin, //psh_id1[mstep_get_net_index()],
		tail //ids ? "" : ".(Rst.process)"
		);
//	printf("%s[%d] ::: ids %02x %02x %02x %02x %02x (%s) chip rev %02x, Chip ID %02x (CS_EACH_MODE)%s\r\n",
//		display_identity_bannerline_title ? display_identity_bannerline_title : display_identity_bannerline_default,
//		mstep_get_net_index(), ids[0], ids[1], ids[2], ids[3], ids[4], DM_GET_DESC(csmode_t, csmode), id_adv, id,
//		ids ? "" : ".(Rst.process)");
	return 0;
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

//char *display_identity_bannerline_title = NULL;
//char *display_identity_bannerline_default =  ": Read device";

//uint16_t psh_id[ETHERNET_COUNT];
//uint8_t psh_ids[ETHERNET_COUNT][5], psh_id_adv[ETHERNET_COUNT];

//int display_identity(char *spiname, uint16_t id, uint8_t *ids, uint8_t id_adv)
//{
//	if (ids) {
//		psh_id[mstep_get_net_index()] = id;
//		memcpy(&psh_ids[mstep_get_net_index()][0], ids, 5);
//		psh_id_adv[mstep_get_net_index()] = id_adv;
//	} else {
//		 id = psh_id[mstep_get_net_index()];
//		 memcpy(ids, &psh_ids[mstep_get_net_index()][0], 5);
//		 id_adv = psh_id_adv[mstep_get_net_index()];
//	}
//
//	printf("%s[%d] ::: ids %02x %02x %02x %02x %02x (%s) chip rev %02x, Chip ID %02x (CS_EACH_MODE)%s\r\n",
//		display_identity_bannerline_title ? display_identity_bannerline_title : display_identity_bannerline_default,
//		mstep_get_net_index(), ids[0], ids[1], ids[2], ids[3], ids[4], DM_GET_DESC(csmode_t, csmode), id_adv, id,
//		ids ? "" : ".(Rst.process)");
//	return 0;
//}

//void display_chipmac(void)
//{
//		uint8_t buf[6];
//		display_identity_bannerline_defaultN = ": rd-bare device";
//		cspi_read_regs(DM9051_PAR, buf, 6, OPT_CS(csmode)); //dm9051opts_csmode_tcsmode()
//		printf("%s[%d] ::: chip-mac %02x%02x%02x%02x%02x%02x\r\n",
//			display_identity_bannerline_title ? display_identity_bannerline_title : display_identity_bannerline_defaultN,
//			mstep_get_net_index(), buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
//}

static void display_rw_mac(char *head, const uint8_t *adr)
{
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	int i;

	printf(": %s[%d] ::: eeprom[] ", head, mstep_get_net_index());
	for (i = 0; i < 3; i++) {
		uint16_t value = dm9051_eeprom_read(i);
		printf("%04x ", value);
	}
	printf("wr-par[] %02x%02x%02x%02x%02x%02x\r\n", adr[0], adr[1], adr[2], adr[3], adr[4], adr[5]);
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

//static char *bare_mac_tbl[2] = {
//	": rd-bare device",
//	": wr-bare device",
//};

//static void display_eeprom_action(char *head)
//{
//#undef printf
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
//	int i;

//	printf("%s[%d] ::: eeprom[] ", 
//		head, //"--EEPROM[%d] word"
//		mstep_get_net_index());

//	for (i = 0; i < 3; i++) {
//		uint16_t value = dm9051_eeprom_read(i);
//		printf("%04x ", value);
//	}
//	bannerline_log();
//#undef printf
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
//}

//static void display_mac_action(char *head, const uint8_t *adr) {
//#undef printf
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))

////	display_mac_bannerline_defaultN = head; // ": rd-bare device";/ ": wr-bare device";
//	printf("%s[%d] ::: chip-mac %02x%02x%02x%02x%02x%02x\r\n",
////				display_identity_bannerline_title ? display_identity_bannerline_title : display_mac_bannerline_defaultN,
//				head,
//				mstep_get_net_index(), adr[0], adr[1], adr[2], adr[3], adr[4], adr[5]);

//#undef printf
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
//}

#define	TIMES_TO_RST	10

void rx_pointer_show(char *headstr)
{
	u16 rwpa_w, mdra_ingress;
	impl_read_rx_pointers(&rwpa_w, &mdra_ingress);
	printf("%s(pin = %d).e rwpa %04x / igrss %04x\r\n",
			headstr, mstep_get_net_index(), rwpa_w, mdra_ingress);
}

u8 ret_fire_time(u8 *histc, int csize, int i, u8 rxb)
{
	u16 rwpa_w, mdra_ingress;
	u8 times = (histc[i] >= TIMES_TO_RST) ? histc[i] : 0;
//	printf(" _dm9051f rxb %02x (times %2d)%c\r\n", rxb, histc[i], (histc[i]==2) ? '*': ' ');

//  rwpa_w = (uint32_t)DM9051_Read_Reg(0x24) | (uint32_t)DM9051_Read_Reg(0x25) << 8; //DM9051_RWPAL
//  mdra_ingress = (uint32_t)DM9051_Read_Reg(0x74) | (uint32_t)DM9051_Read_Reg(0x75) << 8; //DM9051_MRRL;
  impl_read_rx_pointers(&rwpa_w, &mdra_ingress);
  printf("%2d. rwpa %04x / igrss %04x, histc[rxb %02xh], times= %d\r\n",
		 histc[i], rwpa_w, mdra_ingress, rxb, times);

	if (times) { //if (histc[i] >= TIMES_TO_RST)
		dm9051_show_rxbstatistic(histc, csize);
		histc[i] = 1;
		//return times; (return TIMES_TO_RST;)
	}
	return times; //0;
}

const uint8_t *hdlr_reset_process(const uint8_t *macaddr, enable_t en)
{
	dm9051_core_reset(); //Can: printf("rstc %d ,because rxb %02x (is %d times)\r\n", rstc, rxbyte, times);

	if (en) { //As dm9051_init's whole operations. Only for _CH390
		macaddr = dm9051_start1(macaddr);

		display_rw_mac("dm9051_start", macaddr);
//		display_eeprom_action(bare_mac_tbl[0]);
//		display_mac_action(bare_mac_tbl[1], macaddr); //[1]= ": wr-bare device"

//		rx_pointer_show("dm9051_start");
	}
	return macaddr;
}

//static 
u16 ev_rxb(uint8_t rxb)
{
	int i;
	static u8 histc[254] = { 0 }; //static int rff_c = 0 ...;
	u8 times = 1;

	for (i = 0 ; i < sizeof(histc); i++) {
		if (rxb == (i+2)) {
			histc[i]++;
			times = ret_fire_time(histc, sizeof(histc), i, rxb);
			return impl_dm9051_err_hdlr(" : rxbErr %u times :: dm9051_core_reset()\r\n", times, 1); //As: Hdlr (times : TIMES_TO_RST or 0)
			                //: Read device[0] :::
		}
	}
	return impl_dm9051_err_hdlr(" _dm9051f rxb error times (No way!) : %u\r\n", times, 0); //As: Hdlr (times : 1, zerochk : 0)
}

//static
u16 ev_status(uint8_t rx_status)
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
	return impl_dm9051_err_hdlr("_dm9051f rx_status error : 0x%02x\r\n", rx_status, 0);
}

/* if "expression" is true, then execute "handler" expression */

u8 checksum_re_rxb(u8 rxbyte) {
	if (OPT_CONFIRM(rxmode_checksum_offload))
		rxbyte &= 0x03;
	return rxbyte;
}

#if 0
uint16_t dm9051_rx_dump(uint8_t *buff)
{
	// Because we encounter 16-bit mode fail, so went to try above!
	// Since target dm9051a IS only 8-bit mode (working)
	u8 rxbyte, rx_status;
	u8 ReceiveData[4];
	u16 rx_len;
	rxbyte = DM9051_Read_Rxb();
	rxbyte = checksum_re_rxb(rxbyte); //This is because checksum-offload enable

	DM9051_RX_BREAK((rxbyte != 0x01 && rxbyte != 0), return ev_rxb(rxbyte));
	DM9051_RX_BREAK((rxbyte == 0), return 0);

	DM9051_Read_Mem(ReceiveData, 4);
	DM9051_Write_Reg(DM9051_ISR, 0x80);

	rx_status = ReceiveData[1];
	rx_len = ReceiveData[2] + (ReceiveData[3] << 8);

	DM9051_RX_BREAK((rx_status & 0xbf), return ev_status(rx_status)); //_err_hdlr("_dm9051f rx_status error : 0x%02x\r\n", rx_status, 0)
	DM9051_RX_BREAK((rx_len > RX_POOL_BUFSIZE), return impl_dm9051_err_hdlr("_dm9051f rx_len error : %u\r\n", rx_len, 0));

	DM9051_Read_Mem(buff, rx_len);
	DM9051_Write_Reg(DM9051_ISR, 0x80);
	return rx_len;
}
#endif

//static
void dm9051_link_to_hexdump(const void *buffer, size_t len) {
//	#undef printf
//	#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_HEX_DUMP, SEMA_ON, NULL, (fmt, ##__VA_ARGS__))
	#undef printf
	#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	if (dm9051_link_log_reset_hexdump(buffer, len)) {
		uint16_t rwpa_w, mdra_ingress;
		impl_read_rx_pointers(&rwpa_w, &mdra_ingress);
		printf("  rwpa %04x / igrss %04x\r\n", /*rx_modle_count[RX_ANY].allow_num,*/ rwpa_w, mdra_ingress);
	}
	#undef printf
	#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
//	#undef printf
//	#define printf(fmt, ...) TASK_DM9051_DEBUGF(0, SEMA_OFF, "[xx]", (fmt, ##__VA_ARGS__))
}

#if 0
int display_verify_chipid(char *str, char *spiname, uint16_t id) {
	if (_check_chip_id(id)) { //(id == (DM9051_ID/DM9052_ID >> 16))
		chipid_on_pin_code_log_s(spiname, str, id);
	//#if NON_CHIPID_STOP == 1, masked by yicheng
	//	printf("Chip ID CHECK experiment! Succeed OK!!\r\n");
	//	while(1) ; //Feature attribute experiment!!
	//#endif
		return 1;
	} //else {
		chipid_on_pin_code_log_err(spiname, str, id);
		return 0;
	//#if NON_CHIPID_STOP == 1, masked by yicheng
	//	while(1) ;
	//#endif
	//}
}
#endif

int dm9051_init_setup(uint16_t *id)
{
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	uint8_t ids[5], id_adv;
	
	display_identity_bannerline_title = ": dm9051_init";

	first_log_init();

	*id = impl_read_chip_id();
	read_chip_revision(ids, &id_adv);

#if 0 //NO need so 'display'
	display_identity(mstep_spi_conf_name(), *id, ids, *id, id_adv);
#endif

	//display_chipmac();
	DM_SET_FIELD(uint16_t, read_chip_id, *id); //store into dm9051optsex[i].read_chip_id
	
	if (check_chip_id(*id)) {
//		display_eeprom_action();
		return 1;
	}

	return 0;
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

//static void display_baremac(void) {
//	uint8_t buf[6];

//	cspi_read_regs(DM9051_PAR, buf, 6, OPT_CS(csmode)); //dm9051opts_csmode_tcsmode()
//	display_mac_action(bare_mac_tbl[0], buf); //[0]= ": rd-bare device"
//}

int is_multicast_ether_addr(const uint8_t *addr)
{
	return 0x01 & addr[0];
}
int is_zero_ether_addr(const uint8_t *addr)
{
	return (*(const uint16_t *)(addr + 0) |
			*(const uint16_t *)(addr + 2) |
			*(const uint16_t *)(addr + 4)) == 0;
}

int is_valid_ether_addr(const uint8_t *addr)
{
	/* FF:FF:FF:FF:FF:FF is a multicast address so we don't need to
	 * explicitly check for it here. */
	return !is_multicast_ether_addr(addr) && !is_zero_ether_addr(addr);
}

const uint8_t *internal_adr_logic(void)
{
	const uint8_t *adr;
	/*
	 * dm9051 start1 internal logic, i.e.
	 * Read par, if valid then it's from EEPROM
	 * otherwise, use Hard Core Candidate (hcc)
	 */
	//READ MAC_ADDR_LENGTH REGs //CCC
	uint8_t buff[6];
	cspi_read_regs(DM9051_PAR, buff, 6, CS_EACH);
	if (is_valid_ether_addr(buff)) {
//			adr = buff;
		adr = identify_eth_mac(buff, 0); /* Trick1 */
		printf(": EEPROM valid-mac[%d] ::: %02x %02x %02x %02x %02x %02x\r\n",
			mstep_get_net_index(), adr[0],adr[1],adr[2],adr[3],adr[4],adr[5]);
	} else {
		adr = identify_eth_mac(NULL, 0); /* Trick2 */
		printf(": Hard Core candidate-mac[%d] ::: %02x %02x %02x %02x %02x %02x\r\n",
			mstep_get_net_index(), adr[0],adr[1],adr[2],adr[3],adr[4],adr[5]);
	}
	return adr;
}

const uint8_t *dm9051_start1(const uint8_t *adr)
{
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
//	display_baremac();
#if 1
	if (adr) {
		if (is_valid_ether_addr(adr)) {
			adr = identify_eth_mac(adr, 0); /* Trick0 */
//			printf(": APP ENTER-mac[%d] ::: %02x %02x %02x %02x %02x %02x\r\n",
//				mstep_get_net_index(), adr[0],adr[1],adr[2],adr[3],adr[4],adr[5]);
		} else {
			printf(": APP ENTER-mac[%d] ::: %02x %02x %02x %02x %02x %02x (Invalid)\r\n",
				mstep_get_net_index(), adr[0],adr[1],adr[2],adr[3],adr[4],adr[5]);
			adr = internal_adr_logic();
		}
	}
	else
		adr = internal_adr_logic();
#endif
//	display_mac_action(bare_mac_tbl[1], adr); //[1]= ": wr-bare device"

	dm9051_board_irq_enable(NVIC_PRIORITY_GROUP_0); //_dm9051_board_irq_enable();
	dm9051_mac_adr(adr);
	dm9051_rx_mode();
	return adr;
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}
