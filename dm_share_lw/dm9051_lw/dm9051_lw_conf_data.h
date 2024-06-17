//
// dm9051_lw_conf_data.h //2.yc
// - spi configuration data
// - mac addresses data
//
#define board_conf_type	"\"dm9051_at32_mf\""

//-

//-

optsex_t dm9051optsex[BOARD_SPI_COUNT] = { //const
	#define dmopts_normal(iomode, iomode_name) \
		{ \
			/* .set_name */ \
			/*iomode_name,*/ \
			/* .test_plan_include */ \
			/*DM_FALSE,*/ \
			\
			/* to .assign .mac */ \
			{ 0 }, ".assign .mac", \
			/* to .assign .ip.addr */ \
			{ 0 }, ".assign .ip.addr", \
			/* to .assign .gw.addr */ \
			{ 0 }, ".assign .gw.addr", \
			/* to .assign .mask.addr */ \
			{ 0 }, ".assign .mask.addr", \
			\
			/* to .read_chip_id */ \
			0x0000, "read_chip_id", \
			/* .test_plan_log */ \
			DM_TRUE, "some test log", /*DM_FALSE,*/ \
			/* //vs MBNDRY_BYTE, "8-bit",/ MBNDRY_WORD, "16-bit",*/ \
			iomode, iomode_name, \
			/* //vs CS_EACH, "CS_EACH_MODE",/ CS_LONG, "CS_LONG_MODE",*/ \
			CS_EACH, "CS_EACH_MODE", \
			/* //vs. BMCR_RST_DEFAULT, "BMCR PwrOnRst-Default Mode",/ BMCR_FORCE_100MF, "BMCR_Force_100MF_mode"/ BMCR_AUTO_NEG, "BMCR_Auto_Negotiation_mode" */ \
			BMCR_FORCE_100MF, "BMCR_Force_100MF_mode", /*NCR_RST_DEFAULT, "NCR PwrOnRst-Default Mode",*/ /*NCR_AUTO_NEG, "NCR_Auto_Negotiation_mode",*/ \
			/* //vs. 0, "RX_CTRL Normal Mode",/ 1, "RX_CTRL Promiscuos mode" */ \
			0, "RX_CTRL Normal Mode", \
			/* //vs. DM_TRUE, "Normal RX",/ DM_FALSE, "Test RX Loop"*/ \
			DM_FALSE, "Test RX Mode", \
			/* //vs. DM_FALSE, "Checksum offload disable",/ DM_TRUE, "checksum offload enable",*/ \
			DM_TRUE, "rxmode checksum offload enable", \
			/* //vs. DM_FALSE, "Flow control disable",/ DM_TRUE, "Flow control enable",*/ \
			DM_TRUE, "Flow control enable", \
			/* //vs. DM_FALSE, "Device support 8/16 bit modes",/ DM_TRUE, "Device is only 8 bit mode",*/ \
			DM_FALSE, "Device support 8/16 bit modes", \
			/* //vs 0~255, "the delay for x2ms times in the hdlr",*/ \
			150, "The delay for x2ms times in the hdlr", \
			/* //vs. DM_FALSE, "No config set recv",/ DM_TRUE, "Hdlr without configure recv",*/ \
			DM_TRUE, "Hdlr configure recv", \
			/* //vs. DM_TRUE, "Davicom tx endbit",/ DM_FALSE, "No tx endbit",*/ \
			DM_FALSE, "No tx endbit", \
			/* //vs. DM_TRUE, "Generic core rst",/ DM_FALSE, "Traditional core rst",*/ \
			DM_TRUE, "Long_delay core rst", \
		}
	#define dmopts_normaldefault(iomode, iomode_name) \
		{ \
			/* .set_name */ \
			/*iomode_name,*/ \
			/* .test_plan_include */ \
			/*DM_FALSE,*/ \
			\
			/* to .assign .mac */ \
			{ 0 }, ".assign .mac", \
			/* to .assign .ip.addr */ \
			{ 0 }, ".assign .ip.addr", \
			/* to .assign .gw.addr */ \
			{ 0 }, ".assign .gw.addr", \
			/* to .assign .mask.addr */ \
			{ 0 }, ".assign .mask.addr", \
			\
			/* to .read_chip_id */ \
			0x0000, "read_chip_id", \
			/* //.test_plan_log */ \
			DM_FALSE, "some test log", \
			/* //vs MBNDRY_BYTE, "8-bit",/ MBNDRY_WORD, "16-bit",*/ \
			iomode, iomode_name, \
			/* //vs CS_EACH, "CS_EACH_MODE",/ CS_LONG, "CS_LONG_MODE", */ \
			CS_EACH, "CS_EACH_MODE", /*CS_EACH, "CS_EACH_MODE",*/ \
			/* //vs. BMCR_RST_DEFAULT, "BMCR PwrOnRst-Default Mode",/ BMCR_FORCE_100MF, "BMCR_Force_100MF_mode"/ BMCR_AUTO_NEG, "BMCR_Auto_Negotiation_mode" */ \
			BMCR_RST_DEFAULT, "BMCR PwrOnRst-Default Mode", /*NCR_FORCE_100MF, "NCR_Force_100MF_mode",*/ \
			/* //vs. 0, "RX_CTRL Normal Mode",/ 1, "RX_CTRL Promiscuos mode" */ \
			0, "RX_CTRL Normal Mode", /*0, "RX_CTRL Normal Mode",*/ \
			/* //vs. DM_TRUE, "Normal RX",/ DM_FALSE, "Test RX Loop"*/ \
			DM_TRUE, "Normal RX", /*DM_FALSE, "Test RX Mode",*/ \
			/* //vs. DM_FALSE, "Checksum offload disable",/ DM_TRUE, "checksum offload enable", */ \
			DM_FALSE, "Checksum offload disable", \
			/* //vs. DM_FALSE, "Flow control disable",/ DM_TRUE, "Flow control enable", */ \
			DM_FALSE, "Flow control disable", \
			/* //vs. DM_FALSE, "Device support 8/16 bit modes",/ DM_TRUE, "Device is only 8 bit mode",*/ \
			DM_TRUE, "Device is only 8 bit mode", \
			/* //vs 0~255, "the delay for x2ms times in the hdlr",*/ \
			150, "The delay for x2ms times in the hdlr", \
			/* //vs. DM_FALSE, "No config set recv",/ DM_TRUE, "Hdlr without configure recv",*/ \
			DM_TRUE, "Hdlr configure recv", \
			/* //vs. DM_TRUE, "Davicom tx endbit",/ DM_FALSE, "No tx endbit",*/ \
			DM_FALSE, "No tx endbit", \
			/* //vs. DM_TRUE, "Generic core rst",/ DM_FALSE, "Traditional core rst",*/ \
			DM_TRUE, "Long_delay core rst", \
		}
	#define dmopts_test1(iomode, iomode_name) \
		{ \
			/* .set_name */ \
			/*iomode_name,*/ \
			/* .test_plan_include */ \
			/*DM_FALSE,*/ \
			\
			/* to .assign .mac */ \
			{ 0 }, ".assign .mac", \
			/* to .assign .ip.addr */ \
			{ 0 }, ".assign .ip.addr", \
			/* to .assign .gw.addr */ \
			{ 0 }, ".assign .gw.addr", \
			/* to .assign .mask.addr */ \
			{ 0 }, ".assign .mask.addr", \
			\
			/* to .read_chip_id */ \
			0x0000, "read_chip_id", \
			\
			/* below, will fixed to be static const */ \
			\
			/* //.test_plan_log */ \
			DM_FALSE, "some test log", \
			/* //vs MBNDRY_BYTE, "8-bit",/ MBNDRY_WORD, "16-bit",*/ \
			iomode, iomode_name, \
			/* //vs CS_EACH, "CS_EACH_MODE",/ CS_LONG, "CS_LONG_MODE", */ \
			CS_EACH, "CS_EASH_MODE", \
			/* //vs. BMCR_RST_DEFAULT, "BMCR PwrOnRst-Default Mode",/ BMCR_FORCE_100MF, "BMCR_Force_100MF_mode"/ BMCR_AUTO_NEG, "BMCR_Auto_Negotiation_mode" */ \
			BMCR_AUTO_NEG, "BMCR_Auto_Negotiation_mode", \
			/* //vs. 0, "RX_CTRL Normal Mode",/ 1, "RX_CTRL Promiscuos mode" */ \
			0, "RX_CTRL Normal Mode", \
			/* //vs. DM_TRUE, "Normal RX",/ DM_FALSE, "Test RX Loop"*/ \
			DM_FALSE, "Test RX Mode", \
			/* //vs. DM_FALSE, "Checksum offload disable",/ DM_TRUE, "checksum offload enable", */ \
			DM_TRUE, "rxmode checksum offload enable", \
			/* //vs. DM_FALSE, "Flow control disable",/ DM_TRUE, "Flow control enable", */ \
			DM_TRUE, "Flow control enable", \
			/* //vs. DM_FALSE, "Device support 8/16 bit modes",/ DM_TRUE, "Device is only 8 bit mode",*/ \
			DM_FALSE, "Device support 8/16 bit modes", \
			/* //vs 0~255, "the delay for x2ms times in the hdlr",*/ \
			150, "The delay for x2ms times in the hdlr", \
			/* //vs. DM_FALSE, "No config set recv",/ DM_TRUE, "Hdlr without configure recv",*/ \
			DM_TRUE, "Hdlr configure recv", \
			/* //vs. DM_TRUE, "Davicom tx endbit",/ DM_FALSE, "No tx endbit",*/ \
			DM_FALSE, "No tx endbit", \
			/* //vs. DM_TRUE, "Generic core rst",/ DM_FALSE, "Traditional core rst",*/ \
			DM_TRUE, "Long_delay core rst", \
		}
	dmopts_normaldefault(MBNDRY_BYTE, "8-bit"), //CH390 can not use (1, "RX_CTRL Promiscuos mode")
	dmopts_normaldefault(MBNDRY_BYTE, "8-bit"),
	dmopts_normaldefault(MBNDRY_WORD, "16-bit mode"),

	dmopts_normal(MBNDRY_BYTE, "8-bit"),
	dmopts_test1(MBNDRY_WORD, "16-bit mode"),
};


//(define is as rather than '_ETHERNET_COUNT', refer to as '_ETHERNET_COUNT' counter)
const eth_node_t node_candidate[BOARD_SPI_COUNT] = { \
	{ \
		{0, 0x60, 0x6e, 0x00, 0x00, 0x17,}, \
		{192, 168, 6,  17}, \
		{192, 168, 6,   1}, \
		{255, 255, 255, 0}, \
	}, \
	{ \
		{0, 0x60, 0x6e, 0x00, 0x01, 0x26,}, \
		{192, 168, 6,  26}, \
		{192, 168, 6,   1}, \
		{255, 255, 255, 0}, \
	}, \
	{ \
		{0, 0x60, 0x6e, 0x00, 0x01, 0x25,}, \
		{192, 168, 6,  25}, \
		{192, 168, 6,   1}, \
		{255, 255, 255, 0}, \
	}, \
	{ \
		{0, 0x60, 0x6e, 0x00, 0x01, 0xfe,}, \
		{192, 168, 6,  66}, \
		{192, 168, 6,   1}, \
		{255, 255, 255, 0}, \
	}, \
	{ \
		{0, 0x60, 0x6e, 0x00, 0x01, 0xff,}, \
		{192, 168, 6,  67}, \
		{192, 168, 6,   1}, \
		{255, 255, 255, 0}, \
	}, \
};

/*const uint8_t mac_addresse[BOARD_SPI_COUNT][MAC_ADDR_LENGTH] = { \
	{0, 0x60, 0x6e, 0x00, 0x01, 0x17,}, \
	{0, 0x60, 0x6e, 0x00, 0x01, 0x26,}, \
	{0, 0x60, 0x6e, 0x00, 0x01, 0x25,}, \
	{0, 0x60, 0x6e, 0x00, 0x01, 0xff,}, \
	{0, 0x60, 0x6e, 0x00, 0x01, 0xff,}, \
};
const uint8_t local_ipaddr[BOARD_SPI_COUNT][ADDR_LENGTH]   	= { \
	{192, 168, 6, 17}, \
	{192, 168, 6, 26}, \
	{192, 168, 6, 25}, \
};
const uint8_t local_gwaddr[BOARD_SPI_COUNT][ADDR_LENGTH]   	= { \
	{192, 168, 6, 1}, \
	{192, 168, 6, 1}, \
	{192, 168, 6, 1}, \
};
const uint8_t local_maskaddr[BOARD_SPI_COUNT][ADDR_LENGTH] 	= { \
	{255, 255, 255, 0}, \
	{255, 255, 255, 0}, \
	{255, 255, 255, 0}, \
};*/

//
// 'pin_code' always 0. when _ETHERNET_COUNT==1, but _BOARD_SPI_COUNT > 1.
//
int pin_code = 0;

/*IS_DECL_FUNCTION(uint8_t, iomode)
IS_DECL_FUNCTION(uint8_t, promismode)
IS_DECL_FUNCTION(csmode_t, csmode)
IS_DECL_FUNCTION(bmcrmode_t, bmcrmode)
IS_DECL_FUNCTION(enable_t, rxtypemode)
IS_DECL_FUNCTION(enable_t, rxmode_checksum_offload)
IS_DECL_FUNCTION(enable_t, flowcontrolmode)
IS_DECL_FUNCTION(enable_t, onlybytemode);
IS_DECL_FUNCTION(uint8_t, hdir_x2ms)
IS_DECL_FUNCTION(enable_t, hdlr_confrecv)
IS_DECL_FUNCTION(enable_t, tx_endbit)
IS_DECL_FUNCTION(enable_t, generic_core_rst)
*/
//.DECL_SG_FUNCTION(enable_t, test_plan_include)

//#define OPTS_FUNC_IMPL
//#undef OPTS_FUNC_IMPL
	#if 0
	/*#define DM_MACRO2(rtype, field) \
		rtype dm9051opts_##rtype##field(void) { \
			return dm9051optsex[mstep_get_net_index()].##field; \
		} \
		char *dm9051opts_desc##field(void) { \
			return dm9051optsex[mstep_get_net_index()].desc##field##; \
		}*/
	#endif

#define DM_TYPE		2
#include "dm_types.h"

//[common.macro]
#define info_conf_name()			FIELD_SPIDEV(info)
#define cpu_spi_conf_name()			FIELD_SPIDEV(cpu_spi_info) //devconf[pin_code].cpu_api_info
#define cpu_cs_conf_name()			FIELD_SPIDEV(cpu_cs_info)
#define spihead()					FIELD_SPIDEV(spidef)

#define mstep_set_index(i)			pin_code = i //empty for 1 eth project
#define mstep_get_index()			pin_code
#define mstep_turn_net_index()		//empty for 1 eth project

//[common.mac]
/*
 * HCC: Hard Core Candidate (hcc)
 */
#define candidate_eth_mac()			&node_candidate[pin_code].mac_addresse[0]
#define candidate_eth_ip()			&node_candidate[pin_code].local_ipaddr[0]
#define candidate_eth_gw()			&node_candidate[pin_code].local_gwaddr[0]
#define candidate_eth_mask()		&node_candidate[pin_code].local_maskaddr[0]
//#define candidate_eth_mac()		&mac_addresse[pin_code][0]
//#define candidate_eth_ip()		&local_ipaddr[pin_code][0]
//#define candidate_eth_gw()		&local_gwaddr[pin_code][0]
//#define candidate_eth_mask()		&local_maskaddr[pin_code][0]

//-

void mstep_set_net_index(int i)
{
	mstep_set_index(i); //pinCode = i;
}

int mstep_get_net_index(void)
{
	return mstep_get_index();
}

//void mstep_next_net_index(void)
//{
//	mstep_turn_net_index();
//}

char *mstep_spi_conf_name(void)
{
	return spi_conf_name();
}

char *mstep_conf_info(void) {
	return info_conf_name();
}

char *mstep_conf_cpu_spi_ethernet(void) {
	return cpu_spi_conf_name();
}

char *mstep_conf_cpu_cs_ethernet(void) {
	return cpu_cs_conf_name();
}

char *mstep_conf_type(void)
{
	return board_conf_type;
}

// -

#if DM9051OPTS_LOG_ENABLE
void dm9051_opts_iomod_etc(void)
{
#if 0
		int i;
		GpioDisplay();
		for (i = 0; i< ETHERNET_COUNT; i++) {
			mstep_set_net_index(i);
			printf("iomode[%d] %s / value %02x\r\n", mstep_get_net_index(),
					DM_GET_DESC(uint8_t, iomode), OPT_U8(iomode)); //dm9051opts_desciomode(), dm9051opts_iomode()
		}

		for (i = 0; i< ETHERNET_COUNT; i++) {
			mstep_set_net_index(i);
			printf("csmode[%d] %s\r\n", mstep_get_net_index(), DM_GET_DESC(csmode_t, csmode)); //dm9051opts_desccsmode()
		}

		for (i = 0; i< ETHERNET_COUNT; i++) {
			mstep_set_net_index(i);
			printf("bmcmod[%d] %s\r\n", i, DM_GET_DESC(bmcrmode_t, bmcrmode)); //dm9051opts_descbmcrmode()
		}

		for (i = 0; i< ETHERNET_COUNT; i++) {
			mstep_set_net_index(i);
			printf("rxmode[%d] %s\r\n", i, DM_GET_DESC(uint8_t, promismode)); //dm9051opts_descpromismode()
		}

	for (i = 0; i< ETHERNET_COUNT; i++) {
		mstep_set_net_index(i);
		printf("rxtype[%d] %s\r\n", i, DM_GET_DESC(enable_t, rxtypemode)); //dm9051opts_descrxtypemode()
	}

		for (i = 0; i< ETHERNET_COUNT; i++) {
			mstep_set_net_index(i);
			printf("chksum[%d] %s / value %02x %s\r\n", i, DM_GET_DESC(enable_t, rxmode_checksum_offload), //dm9051opts_descrxmode_checksum_offload(), ~dm9051opts_desc_rxchecksum_offload(),
					OPT_CONFIRM(rxmode_checksum_offload) ? RCSSR_RCSEN : 0,
					OPT_CONFIRM(rxmode_checksum_offload) ? "(RCSSR_RCSEN)" : " "); //is_dm9051opts_rxmode_checksum_offload ~is_dm9051opts_rxchecksum_offload
		}

		for (i = 0; i< ETHERNET_COUNT; i++) {
			mstep_set_net_index(i);
			printf("fcrmod[%d] %s / value %02x\r\n", i, DM_GET_DESC(enable_t, flowcontrolmode), //dm9051opts_descflowcontrolmode()
					OPT_CONFIRM(flowcontrolmode) ? FCR_DEFAULT_CONF : 0); //(_dm9051optsex[mstep_get_net_index()]._flowcontrolmode)
		}

		for (i = 0; i< ETHERNET_COUNT; i++) {
			mstep_set_net_index(i);
			printf("iomode[%d] %s %s\r\n", i,
					OPT_CONFIRM(onlybytemode) ? "device: " : "set-to: ",
					OPT_CONFIRM(onlybytemode) ? DM_GET_DESC(enable_t, onlybytemode) : DM_GET_DESC(uint8_t, iomode));
												//dm9051opts_desconlybytemode() : dm9051opts_desciomode()
		}

//	#if TO_ADD_CODE_LATER_BACK
//		for (i = 0; i< ETHERNET_COUNT; i++) {
//			uint8_t *macaddr;
//			mstep_set_net_index(i);
//			macaddr = mstep_eth_mac();
//			printf("config tobe mac[%d] %02x%02x%02x%02x%02x%02x\r\n", i, macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
//		}
//	#endif
	#if TO_ADD_CODE_LATER_BACK
		for (i = 0; i< ETHERNET_COUNT; i++) {
			mstep_set_net_index(i);
			lEepromDisplay(i);
		}
	#endif
	#if TO_ADD_CODE_LATER_BACK
		for (i = 0; i< ETHERNET_COUNT; i++) {
			mstep_set_net_index(i);
			printf("ip[%d] %"U16_F".%"U16_F".%"U16_F".%"U16_F"\r\n", i,
			  ip4_addr1_16(netif_ip4_addr(&xnetif[i])),
			  ip4_addr2_16(netif_ip4_addr(&xnetif[i])),
			  ip4_addr3_16(netif_ip4_addr(&xnetif[i])),
			  ip4_addr4_16(netif_ip4_addr(&xnetif[i])));
		}
	#endif
#endif
}
#endif

const uint8_t *identify_eth_mac(const uint8_t *macadr, int showflg) {
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	const uint8_t *mac;

	DM_SET_FIELD(mac_t, final_mac, macadr ? macadr : candidate_eth_mac()); //determine which one, to set to field.
	mac = DM_GET_FIELD(mac_t, final_mac);

	if (showflg)
		printf("dm9051_init[%d] %s %s, %s, %s, to set mac/ %02x%02x%02x%02x%02x%02x\r\n",
			mstep_get_net_index(),
			mstep_conf_info(),
			mstep_spi_conf_name(), //spi_conf_name(),
			mstep_conf_cpu_spi_ethernet(),
			mstep_conf_cpu_cs_ethernet(),
			mac[0],
			mac[1],
			mac[2],
			mac[3],
			mac[4],
			mac[5]);
	return mac;
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

uint8_t *identify_tcpip_ip(uint8_t *ip4adr) {
	DM_SET_FIELD(ip_t ,ip, ip4adr ? ip4adr : candidate_eth_ip());
	return DM_GET_FIELD(ip_t, ip);
}
uint8_t *identify_tcpip_gw(uint8_t *ip4adr) {
	DM_SET_FIELD(ip_t ,gw, ip4adr ? ip4adr : candidate_eth_gw());
	return DM_GET_FIELD(ip_t, gw);
}
uint8_t *identify_tcpip_mask(uint8_t *ip4adr) {
	DM_SET_FIELD(ip_t ,mask, ip4adr ? ip4adr : candidate_eth_mask());
	return DM_GET_FIELD(ip_t, mask);
}

//#define FREERTOS_ETHERNETIF_MAC_ADDR	1 //(netconf.h)
//extern uint8_t MACaddr[6];
//#if FREERTOS_ETHERNETIF_MAC_ADDR
//	return MACaddr;
//#else
//#endif

//const
uint8_t *mstep_eth_mac(void) {
	return DM_GET_FIELD(mac_t, final_mac);
}

//const
uint8_t *mstep_eth_ip(void) {
	return DM_GET_FIELD(ip_t, ip);
}
//const
uint8_t *mstep_eth_gw(void) {
	return DM_GET_FIELD(ip_t, gw);
}
//const
uint8_t *mstep_eth_mask(void) {
	return DM_GET_FIELD(ip_t, mask);
}

//-
bmcrmode_t mstep_opts_bmcrmode(void) {
	return DM_GET_FIELD(bmcrmode_t, bmcrmode);
}

//-

//uint32_t extline_A(void) {
//	return exint_extline();
//}
