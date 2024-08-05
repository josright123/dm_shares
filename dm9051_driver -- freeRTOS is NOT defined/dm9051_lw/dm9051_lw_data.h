//.=#include "dm9051_lw_conf_data.h" //1.yicheng 2.later than 'board_printf'
//.=#include "dm9051_lw_conf_data0.h" //1.yicheng 2.later than 'board_printf'

#if 1
/*
 * structure declaration
 */
typedef struct {
#define DM_TYPE		1
#include "dm_types1.h"
} optsex_t;
#endif

/*
 * candidate
 */
typedef struct eth_node_st {
  uint8_t mac_addresse[MAC_ADDR_LENGTH];
  uint8_t local_ipaddr[ADDR_LENGTH];
  uint8_t local_gwaddr[ADDR_LENGTH];
  uint8_t local_maskaddr[ADDR_LENGTH];
} eth_node_t;

optsex_t dm9051optsex[BOARD_SPI_COUNT] = { //const
	#define dmopts_normal(iomode, iomode_name) \
		{ \
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
			0x0000, ".irqst (irq_status)", \
		}
	#define dmopts_normaldefault(iomode, iomode_name) \
		{ \
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
			0x0000, ".irqst (irq_status)", \
		}
	#define dmopts_test1(iomode, iomode_name) \
		{ \
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
			0x0000, ".irqst (irq_status)", \
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

#define DM_TYPE		2
#include "dm_types1.h"

/*
 * HCC: Hard Core Candidate (hcc)
 */
#define candidate_eth_mac()			&node_candidate[0].mac_addresse[0] //[pin_code]
#define candidate_eth_ip()			&node_candidate[0].local_ipaddr[0] //[pin_code]
#define candidate_eth_gw()			&node_candidate[0].local_gwaddr[0] //[pin_code]
#define candidate_eth_mask()		&node_candidate[0].local_maskaddr[0] //[pin_code]

const uint8_t *identify_eth_mac(const uint8_t *macadr, int showflg) {
	//#undef printf
	//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	const uint8_t *mac;
	DM_SET_FIELD(mac_t, final_mac, macadr ? macadr : candidate_eth_mac()); //determine which one, to set to field.
	mac = DM_GET_FIELD(mac_t, final_mac);

	if (showflg)
		printf("identify_eth_mac[%d] to set mac/ %02x%02x%02x%02x%02x%02x\r\n",
				0, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	return mac;
	//#undef printf
	//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

uint8_t *identified_eth_mac(void) {
	return DM_GET_FIELD(mac_t, final_mac);
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

uint8_t *identified_tcpip_ip(void) {
	return DM_GET_FIELD(ip_t, ip);
}
uint8_t *identified_tcpip_gw(void) {
	return DM_GET_FIELD(ip_t, gw);
}
uint8_t *identified_tcpip_mask(void) {
	return DM_GET_FIELD(ip_t, mask);
}

//.=#include "dm9051_lw_conf_data1.h" //1.yicheng 2.later than 'board_printf'

//.int pin_code = 0;

void mstep_set_net_index(int i){
	(void) i;
	//pinCode = i;
}

int mstep_get_net_index(void){
	return 0; //= pinCode
}

void identify_chip_id(uint16_t pid) {
	DM_SET_FIELD(uint16_t, read_chip_id, pid); //store into dm9051optsex[i].read_chip_id
}

uint16_t identified_chip_id(void) {
	return DM_GET_FIELD(uint16_t, read_chip_id);
}

void deidentify_irq_stat(uint16_t bitflg) {
	DM_SET_FIELD(uint16_t, irqst, DM_GET_FIELD(uint16_t, irqst) & ~bitflg);
}

void identify_irq_stat(uint16_t bitflg) {
	DM_SET_FIELD(uint16_t, irqst, DM_GET_FIELD(uint16_t, irqst) | bitflg); //store into dm9051optsex[i].read_chip_id
}

uint16_t identified_irq_stat(void) {
	return DM_GET_FIELD(uint16_t, irqst);
}
