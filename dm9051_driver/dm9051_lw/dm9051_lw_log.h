/**
  **************************************************************************
  * @file     _dm9051_lw_log.h
  * @version  v1.0
  * @date     2023-04-28
  * @brief    header file of dm9051 environment config program.
  **************************************************************************
  */
#ifndef __DM9051_DBG_H
#define __DM9051_DBG_H

#define RX_ANY	0
#define RX_MON	1
#define RX_DUMP_NUM	2

typedef struct { //different purpose to //"dm9051_lw_log_types.h"
	int allow_num;
} pkt_monitor_t;

extern const pkt_monitor_t rx_modle[RX_DUMP_NUM]; //const
extern pkt_monitor_t rx_modle_count[RX_DUMP_NUM]; //varible

void lEepromDisplay(int pin);

//typedef struct {
//	int allow_num;
//} pkt_monitor_t;

//#define TX_MODLE_DECLARATION \
//const pkt_monitor_t tx_modle = { \
//	16, \
//}; \
//const pkt_monitor_t tx_all_modle = { \
//	0, \
//}

//void dm9051_link_log_reset(void);
int dm9051_link_log_reset_hexdump(const void *buffer, size_t len);

/* Put here, instead. Instead of "dm9051_lw_log.h", some generic called in the other implementation place.
 */
#if DM9051OPTS_API
#if DM9051_DEBUG_ENABLE == 1
#define bannerline_log()   printf("\r\n")  //driver provide a macro definition, only. (NOT funcation API function.)
void dm9051_txlog_monitor_tx(int hdspc, const uint8_t *buffer, size_t len);
void dm9051_rxlog_monitor_rx_all(int hdspc, const uint8_t *buffer, size_t len);
void dm9051_rxlog_monitor_rx(int hdspc, char *cause_str, const uint8_t *buffer, size_t len);
void dm9051_txlog_monitor_tx_all(int hdspc, const uint8_t *buffer, size_t len);
void dm9051_txlog_disp(uint8_t *buffer, int len);
//void dm9051_log_dump0(char *prefix_str, size_t tlen, const void *buf, size_t len);
void dm9051_rxlog_ack(void *payload, uint16_t tot_len, char *result_str);
void dm9051_rxlog_arp(void *payload, uint16_t tot_len, uint16_t len);
#else
#define bannerline_log()
#define dm9051_txlog_monitor_tx( hdspc,   buffer,  len)
#define dm9051_txlog_monitor_tx_all( hdspc,   buffer,  len)
#define dm9051_rxlog_monitor_rx( hdspc,	cause_str,   buffer,  len)
#define dm9051_rxlog_monitor_rx_all( hdspc,   buffer,  len)
#define dm9051_txlog_disp(buffer,  len)
//.#define dm9051_log_dump0(prefix_str, tlen, buf, len)
#define dm9051_rxlog_ack( payload,  tot_len,  result_str)
#define dm9051_rxlog_arp( payload,  tot_len,  len)
#endif
#endif

void dm9051_disp_tx(const uint8_t *buf, uint16_t len);
int dm9051_disp_and_check_rx(const uint8_t *buf, uint16_t len);

void dm9051_rx_unknow_pkt_inc_count(void);
void dm9051_rx_unknow_pkt_display_count(void);

#endif //__DM9051_DBG_H
