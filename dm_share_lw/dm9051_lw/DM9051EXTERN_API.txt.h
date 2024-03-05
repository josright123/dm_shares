//[dm9051_lw.o]
#if DM9051OPTS_API
uint16_t dm9051_init(const uint8_t *adr);
uint16_t dm9051_rx(uint8_t *buff);
void dm9051_tx(uint8_t *buf, uint16_t len);
void hdlr_reset_process(enable_t en);
void dm9051_mac_adr(const uint8_t *macadd);
uint16_t dm9051_bmsr_update(void);
uint16_t dm9051_phy_read(uint32_t reg);
uint16_t dm9051_eeprom_read(uint16_t word);
int check_chip_id(uint16_t id);
#endif
//[dm9051_lw_conf.o]
//init
#if DM9051OPTS_API
/*(main.o)*/ void dm9051_boards_initialize(int n);
void identify_dm9051_mac(uint8_t *macadr);
void identify_lwip_ip(uint8_t *ip4adr);
void identify_lwip_gw(uint8_t *ip4adr);
void identify_lwip_mask(uint8_t *ip4adr);
void mstep_set_net_index(int i);
void mstep_next_net_index(void);
char *mstep_spi_conf_name(void);
char *mstep_conf_cpu_spi_ethernet(void);
#endif

//[dm9051_lw_log.o]
#if DM9051OPTS_API
#if DM9051_DEBUG_ENABLE == 1
#define bannerline_log()   printf("\r\n")
void dm9051_txlog_monitor_tx(int hdspc, const uint8_t *buffer, size_t len);
void dm9051_rxlog_monitor_rx_all(int hdspc, const uint8_t *buffer, size_t len);
void dm9051_rxlog_monitor_rx(int hdspc, const uint8_t *buffer, size_t len);
void dm9051_txlog_monitor_tx_all(int hdspc, const uint8_t *buffer, size_t len);
void dm9051_txlog_disp(uint8_t *buffer, int len);
//void dm9051_log_dump0(char *prefix_str, size_t tlen, const void *buf, size_t len);
void dm9051_rxlog_ack(void *payload, uint16_t tot_len, char *result_str);
void dm9051_rxlog_arp(void *payload, uint16_t tot_len, uint16_t len);
#endif
#endif
//[dm9051opts.o]
#if DM9051OPTS_API
typedef uint16_t (* trn_conn_t)(int i);
int TRANS_DUAL(trn_conn_t f); //return : found id number
/*(main.o)*/ void dm9051_opts_display(void);
#endif
#if DM9051OPTS_LOG_ENABLE
/*(main.o)*/ void dm9051_opts_iomod_etc(void);
#endif
//[dmtypes.h]
#if DM_TYPE == 0
// - type 0, extern API
/*(main.o)*/ dm9051opts_descrxtypemode //char *dm9051opts_desc##field(void); 
/*(main.o)*/ dm9051opts_enable_trxtypemode //rtype dm9051opts_##mtype##field(void); 
#endif