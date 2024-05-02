//mqttc
linking...
//[dm9051_lw_conf.o]
/*(main.o)*/  dm9051_boards_initialize (referred from main.o).
/* mqtt_client.o */ mstep_set_net_index (referred from mqtt_client.o).
/* netconf_b.o */ mstep_get_net_index (referred from netconf_b.o).
/* stack_init.o */ mstep_eth_mac (referred from stack_init.o).
/* developer_conf.o */ mstep_eth_gw (referred from developer_conf.o).
/* developer_conf.o */ mstep_eth_ip (referred from developer_conf.o).
/* developer_conf.o */ mstep_eth_mask (referred from developer_conf.o).
//[dm9051opts.o]
/*(mqtt_main.o)*/  ETH_COUNT_VOIDFN (referred from mqtt_main.o).
/* ethernetif.o */ ETH_COUNT_VOIDTX (referred from ethernetif.o).
//[dm9051_lw.o]
/* ethernetif.o */ dm9051_init (referred from ethernetif.o).
/* ethernetif.o */ dm9051_rx (referred from ethernetif.o).
/* ethernetif.o */ dm9051_tx (referred from ethernetif.o).
/* ethernetif.o */ check_chip_id (referred from ethernetif.o).
/* netconf_b.o */ dm9051_bmsr_update (referred from netconf_b.o).
//[dm9051_lw_log.o]
/* ethernetif.o */ dm9051_rxlog_ack (referred from ethernetif.o).
/* ethernetif.o */ dm9051_rxlog_arp (referred from ethernetif.o).
/* ethernetif.o */ dm9051_rxlog_monitor_rx_all (referred from ethernetif.o).
/* ethernetif.o */ dm9051_txlog_disp (referred from ethernetif.o).
/* ethernetif.o */ dm9051_txlog_monitor_tx (referred from ethernetif.o).
//[dmtypes.h]
#if DM_TYPE == 0
/* netif.o */ dm9051opts_enable_ttest_plan_log //rtype dm9051opts_##mtype##field(void); //DM_GET_FIELD()
/* netif.o */ (dm9051opts_desctest_plan_log (referred from netif.o).) //char *dm9051opts_desc##field(void); //DM_GET_DESC
/* linkhdlr_timeouts.o */ dm9051opts_descbmcrmode (referred from linkhdlr_timeouts.o). //char *dm9051opts_desc##field(void); //DM_GET_DESC()
#endif

/* linkhdlr_timeouts.o */  dm9051_bmcr_update (referred from linkhdlr_timeouts.o).
/* ethernetif.o */ cspi_read_reg (referred from ethernetif.o).
/* comm.o */ mstep_next_net_index (referred from netconf - comm.o).
/* ethernetif.o */ mstep_spi_conf_name (referred from ethernetif.o).
/* log.o */ GpioDisplay (referred from netcon2 - log.o).

//X2Web
//[dm9051_lw.o]
#if DM9051OPTS_API
/*(proc_main.o)*/ uint16_t dm9051_init(const uint8_t *adr);
/*(netconf.o)*/ uint16_t dm9051_rx(uint8_t *buff);
/*(ethernetif.o)*/ void dm9051_tx(uint8_t *buf, uint16_t len);
/*(ethernetif.o)*/ void hdlr_reset_process(enable_t en);
/*(proc_main.o)*/ void dm9051_mac_adr(const uint8_t *macadd);
/*(proc_main.o)*/ uint16_t dm9051_bmsr_update(void);
/*(ethernetif.o)*/ int check_chip_id(uint16_t id);
#endif
/*(netconf.o)*/ uint16_t dm9051_phy_read(uint32_t reg);
/*(proc_main.o)*/ uint16_t dm9051_bmcr_update(void);
/*(proc_main.o)*/ uint16_t dm9051_rx_dump(uint8_t *buff);
/*(proc_main.o)*/ char *display_identity_bannerline_title;

//[dm9051_lw_conf.o]
//init
#if DM9051OPTS_API
/*(main.o)*/ void dm9051_boards_initialize(int n);
/*(stack_init.o)*/ uint8_t *identify_eth_mac(uint8_t *macadr)
/*(stack_init.o)*/ uint8_t *identify_tcpip_ip(uint8_t *ip4adr);
/*(stack_init.o)*/ uint8_t *identify_tcpip_gw(uint8_t *ip4adr);
/*(stack_init.o)*/ uint8_t *identify_tcpip_mask(uint8_t *ip4adr)
/*(main.o)*/ void mstep_set_net_index(int i);
/*(proc_main.o)*/ int mstep_get_net_index(void);
/*(at32f435_437_int.o)*/ void dm9051_irqlines_proc(void);
#endif
/*(proc_main.o)*/ void dm_delay_ms(uint16_t nms);
/*(proc_main.o)*/ uint8_t *mstep_eth_mac(void);
/*(developer_conf.o)*/ uint8_t *mstep_eth_ip(void);
/*(developer_conf.o)*/ uint8_t *mstep_eth_gw(void);
/*(developer_conf.o)*/ uint8_t *mstep_eth_mask(void);
/*(proc_main.o)*/ cspi_read_reg
/*(proc_main.o)*/ cspi_read_regs
/*(proc_main.o)*/ cspi_write_reg

//[dm9051_lw_log.o]
#if DM9051OPTS_API
#if DM9051_DEBUG_ENABLE == 1
/*(ooooooo.o)*/ #define bannerline_log()   printf("\r\n")
/*(netifo.o)*/ void dm9051_txlog_monitor_tx(int hdspc, const uint8_t *buffer, size_t len);
/*(netconf.o)*/ void dm9051_rxlog_monitor_rx_all(int hdspc, const uint8_t *buffer, size_t len);
/*(xxxxxxx.o)*/ void dm9051_rxlog_monitor_rx(int hdspc, const uint8_t *buffer, size_t len);
/*(ethernetif.o)*/ void dm9051_txlog_monitor_tx_all(int hdspc, const uint8_t *buffer, size_t len);
/*(ethernetif.o)*/ void dm9051_txlog_disp(uint8_t *buffer, int len);
/*(ethernetif.o)*/ void dm9051_rxlog_ack(void *payload, uint16_t tot_len, char *result_str);
/*(ethernetif.o)*/ void dm9051_rxlog_arp(void *payload, uint16_t tot_len, uint16_t len);
#endif
#endif
//[dm9051opts.o]
#if DM9051OPTS_API
//.int TRANS_DUAL(trn_conn_t f); //return : found id number
/*(main.o)*/ void dm9051_opts_display(void);
#endif
#if DM9051OPTS_LOG_ENABLE
/*(main.o)*/ void dm9051_opts_iomod_etc(void);
#endif
/*(log.o)*/ void ethcnt_ifdiplay(void);
//[dmtypes.h]
#if DM_TYPE == 0
// - type 0, extern API
/*(main.o)*/ dm9051opts_enable_trxtypemode //rtype dm9051opts_##mtype##field(void);
/*(main.o)*/ dm9051opts_descrxtypemode //char *dm9051opts_desc##field(void);
/*(proc_main.o)*/ dm9051opts_enable_ttest_plan_log //rtype dm9051opts_##mtype##field(void);
/*(proc_main.o)*/ dm9051opts_csmode_tcsmode //rtype dm9051opts_##mtype##field(void);
/*(proc_main.o)*/ dm9051opts_enable_tflowcontrolmode //rtype dm9051opts_##mtype##field(void);
/*(proc_main.o)*/ dm9051opts_descflowcontrolmode //rtype dm9051opts_##mtype##field(void);
/*(netconf.o)*/ dm9051opts_bmcrmode_tbmcrmode //rtype dm9051opts_##mtype##field(void);
/*(ethernetif.o)*/ dm9051opts_enable_ttx_endbit //rtype dm9051opts_##mtype##field(void);
#endif



x2web start: [BOARD_SPI COUNT] 5  /  Operating: [ETHERNET COUNT] 1
@AT32F437 ETHERNET SPI4, sck/mi/mo/ pe2/pe5/pe6, cs/ pe4

dm9051[0]_options_display:
 - core rst mode, Long_delay core rst
 - tx_endbit mode, No tx endbit
@AT32F437 ETHERNET SPI4, sck/mi/mo/ pe2/pe5/pe6, cs/ pe4
iomode[0] 8-bit / value 80
csmode[0] CS_EACH_MODE
bmcmod[0] BMCR PwrOnRst-Default Mode
rx's mode[0] RX_CTRL Normal Mode
rxtype[0] Normal RX
chksum[0] Checksum offload disable / value 00
fcrmod[0] Flow control disable / value 00
iomode[0] device:  Device is only 8 bit mode
.###. init [ 20007970 = malloc( 15 ) /for startup test /for a reference can according to]

: lwip_init()

  ! Lw[0] ARP event/ bmsr 7849, but only well on link-up /or addr-chang

BannerDisplay() MEMP_MAX= 10



dm9051_tmr_init()
while(1), waiting...
bm[0] bmcrmode 0, bmc 3100
waiting, waiting[0]/done-flg=0 ...
bms[0][00] bms 7849, timeout 0..
bms[0][00] bms 7849, timeout 1..
bms[0][00] bms 7849, timeout 2..
wait-complete, waiting[0]/done-flg=0 ...
...................... ........................s .......................
... Start.s nsr 80
... Start.e nsr 80
rxtype[0] val 1
rxtype[0] des Normal RX
...................... ........................e .......................
...................... lwip_reinitialize()...run .......................

----- : sys_timeouts_init() firstly initialize 2 timers
----- : 1. new-timeouts  1000ms ip_reass_tmr
----- : 2. new-timeouts  1000ms etharp_tmr
  ! Lw[0] ARP event/ bmsr 7849, arp can PING

  (1/5 tx[0]>>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 01 17 08 00 0e 0f   10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f
  (    tx[0]>>) 00000020:  20 21 22 23 24 25 26 27  28 29
  (2/5 tx[0]>>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 01 17 08 00 0e 0f   10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f
  (    tx[0]>>) 00000020:  20 21 22 23 24 25 26 27  28 29 2a
  (3/5 tx[0]>>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 01 17 08 00 0e 0f   10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f
  (    tx[0]>>) 00000020:  20 21 22 23 24 25 26 27  28 29 2a 2b
  (4/5 tx[0]>>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 01 17 08 00 0e 0f   10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f
  (    tx[0]>>) 00000020:  20 21 22 23 24 25 26 27  28 29 2a 2b 2c
  (5/5 tx[0]>>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 01 17 08 00 0e 0f   10 11 12 13 14 15 16 17  18 19 1a 1b 1c 1d 1e 1f
  (    tx[0]>>) 00000020:  20 21 22 23 24 25 26 27  28 29 2a 2b 2c 2d
dm9051[0] up, 192.168.6.33 (bmsr 786d)
dm9051[0] link up
tcp_output_segment.s
ip_output_if.s
ip_output_if.e
tcp_output_segment.e
  (<TCP>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 00 33 08 00 45 00   00 2c 00 00 00 00 ff 06  2e 28 c0 a8 06 21 c0 a8
  (<TCP>) 00000020:  06 32 00 50 fc e1 00 00  19 6e c4 2b 38 6b 60 12   16 d0
 ..SrcIP 192.168.6.33  DestIP 192.168.6.50  Len 58  (   80 -> 64737) flags 12
tcp_output.s
tcp_output.e
  (dm9 monitor <<rx  ) 00000000:  00 60 6e 00 00 33 08 97  98 c3 b9 a2 08 00 45 00   02 19 ae ce 40 00 80 06  bc 6c c0 a8 06 32 c0 a8
  (dm9 monitor <<rx  ) 00000020:  06 21 fc e1 00 50 c4 2b  38 6b 00 00 19 6f 50 18   fa f0 cb 47 00 00 47 45  54 20 2f 20 48 54 54 50
  (dm9 monitor <<rx  ) 00000040:  2f 31 2e 31 0d 0a
 ..SrcIP 192.168.6.50  DestIP 192.168.6.33  Len 555  (64737 -> 80) flags 18 (PSH, ACK)
tcp_output.s
tcp_output_segment.s
ip_output_if.s
dm9051_tx_.s ,len 1514 (LARGE-SIZE)
  (<TCP>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 00 33 08 00 45 00   05 dc 00 01 00 00 ff 06  28 77 c0 a8 06 21 c0 a8
  (<TCP>) 00000020:  06 32 00 50 fc e1 00 00  19 6f c4 2b 3a 5c 50 10   14 df
 ..SrcIP 192.168.6.33  DestIP 192.168.6.50  Len 1514  (   80 -> 64737) flags 10
ip_output_if.e
tcp_output_segment.e
tcp_output_segment.s
ip_output_if.s
dm9051_tx_.s ,len 755 (LARGE-SIZE)
  (<TCP>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 00 33 08 00 45 00   02 e5 00 02 00 00 ff 06  2b 6d c0 a8 06 21 c0 a8
  (<TCP>) 00000020:  06 32 00 50 fc e1 00 00  1f 23 c4 2b 3a 5c 50 19   14 df
 ..SrcIP 192.168.6.33  DestIP 192.168.6.50  Len 755  (   80 -> 64737) flags 19
ip_output_if.e
tcp_output_segment.e
tcp_output.e
tcp_output_segment.s
ip_output_if.s
  (<TCP>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 00 33 08 00 45 00   00 2c 00 03 00 00 ff 06  2e 25 c0 a8 06 21 c0 a8
  (<TCP>) 00000020:  06 32 00 50 fc e2 00 00  19 6e c6 eb 90 cf 60 12   16 d0
 ..SrcIP 192.168.6.33  DestIP 192.168.6.50  Len 58  (   80 -> 64738) flags 12
ip_output_if.e
tcp_output_segment.e
tcp_output.s
tcp_output.e
tcp_output.s
  (<TCP>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 00 33 08 00 45 00   00 28 00 04 00 00 ff 06  2e 28 c0 a8 06 21 c0 a8
  (<TCP>) 00000020:  06 32 00 50 fc e1 00 00  21 e1 c4 2b 3a 5d 50 10   14 de
 ..SrcIP 192.168.6.33  DestIP 192.168.6.50  Len 54  (   80 -> 64737) flags 10
tcp_output.e
tcp_output.s
tcp_output.e
  (dm9 monitor <<rx  ) 00000000:  00 60 6e 00 00 33 08 97  98 c3 b9 a2 08 00 45 00   01 c1 ae d3 40 00 80 06  bc bf c0 a8 06 32 c0 a8
  (dm9 monitor <<rx  ) 00000020:  06 21 fc e2 00 50 c6 eb  90 cf 00 00 19 6f 50 18   fa f0 32 a9 00 00 47 45  54 20 2f 6c 6f 67 6f 2e
  (dm9 monitor <<rx  ) 00000040:  6a 70 67 20 48 54
 ..SrcIP 192.168.6.50  DestIP 192.168.6.33  Len 467  (64738 -> 80) flags 18 (PSH, ACK)
tcp_output.s
tcp_output_segment.s
ip_output_if.s
dm9051_tx_.s ,len 1514 (LARGE-SIZE)
  (<TCP>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 00 33 08 00 45 00   05 dc 00 05 00 00 ff 06  28 73 c0 a8 06 21 c0 a8
  (<TCP>) 00000020:  06 32 00 50 fc e2 00 00  19 6f c6 eb 92 68 50 10   15 37
 ..SrcIP 192.168.6.33  DestIP 192.168.6.50  Len 1514  (   80 -> 64738) flags 10
ip_output_if.e
tcp_output_segment.e
tcp_output_segment.s
ip_output_if.s
dm9051_tx_.s ,len 1514 (LARGE-SIZE)
  (<TCP>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 00 33 08 00 45 00   05 dc 00 06 00 00 ff 06  28 72 c0 a8 06 21 c0 a8
  (<TCP>) 00000020:  06 32 00 50 fc e2 00 00  1f 23 c6 eb 92 68 50 18   15 37
 ..SrcIP 192.168.6.33  DestIP 192.168.6.50  Len 1514  (   80 -> 64738) flags 18 (PSH, ACK)
ip_output_if.e
tcp_output_segment.e
tcp_output.e
tcp_output.s
tcp_output_segment.s
ip_output_if.s
dm9051_tx_.s ,len 1514 (LARGE-SIZE)
  (<TCP>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 00 33 08 00 45 00   05 dc 00 07 00 00 ff 06  28 71 c0 a8 06 21 c0 a8
  (<TCP>) 00000020:  06 32 00 50 fc e2 00 00  24 d7 c6 eb 92 68 50 10   15 37
 ..SrcIP 192.168.6.33  DestIP 192.168.6.50  Len 1514  (   80 -> 64738) flags 10
ip_output_if.e
tcp_output_segment.e
tcp_output_segment.s
ip_output_if.s
dm9051_tx_.s ,len 1514 (LARGE-SIZE)
  (<TCP>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 00 33 08 00 45 00   05 dc 00 08 00 00 ff 06  28 70 c0 a8 06 21 c0 a8
  (<TCP>) 00000020:  06 32 00 50 fc e2 00 00  2a 8b c6 eb 92 68 50 18   15 37
 ..SrcIP 192.168.6.33  DestIP 192.168.6.50  Len 1514  (   80 -> 64738) flags 18 (PSH, ACK)
ip_output_if.e
tcp_output_segment.e
tcp_output.e
tcp_output.s
tcp_output_segment.s
ip_output_if.s
dm9051_tx_.s ,len 1514 (LARGE-SIZE)
  (<TCP>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 00 33 08 00 45 00   05 dc 00 09 00 00 ff 06  28 6f c0 a8 06 21 c0 a8
  (<TCP>) 00000020:  06 32 00 50 fc e2 00 00  30 3f c6 eb 92 68 50 10   15 37
 ..SrcIP 192.168.6.33  DestIP 192.168.6.50  Len 1514  (   80 -> 64738) flags 10
ip_output_if.e
tcp_output_segment.e
tcp_output_segment.s
ip_output_if.s
dm9051_tx_.s ,len 1514 (LARGE-SIZE)
  (<TCP>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 00 33 08 00 45 00   05 dc 00 0a 00 00 ff 06  28 6e c0 a8 06 21 c0 a8
  (<TCP>) 00000020:  06 32 00 50 fc e2 00 00  35 f3 c6 eb 92 68 50 18   15 37
 ..SrcIP 192.168.6.33  DestIP 192.168.6.50  Len 1514  (   80 -> 64738) flags 18 (PSH, ACK)
ip_output_if.e
tcp_output_segment.e
tcp_output.e
tcp_output.s
tcp_output_segment.s
ip_output_if.s
dm9051_tx_.s ,len 1514 (LARGE-SIZE)
  (<TCP>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 00 33 08 00 45 00   05 dc 00 0b 00 00 ff 06  28 6d c0 a8 06 21 c0 a8
  (<TCP>) 00000020:  06 32 00 50 fc e2 00 00  3b a7 c6 eb 92 68 50 10   15 37
 ..SrcIP 192.168.6.33  DestIP 192.168.6.50  Len 1514  (   80 -> 64738) flags 10
ip_output_if.e
tcp_output_segment.e
tcp_output_segment.s
ip_output_if.s
dm9051_tx_.s ,len 1514 (LARGE-SIZE)
  (<TCP>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 00 33 08 00 45 00   05 dc 00 0c 00 00 ff 06  28 6c c0 a8 06 21 c0 a8
  (<TCP>) 00000020:  06 32 00 50 fc e2 00 00  41 5b c6 eb 92 68 50 18   15 37
 ..SrcIP 192.168.6.33  DestIP 192.168.6.50  Len 1514  (   80 -> 64738) flags 18 (PSH, ACK)
ip_output_if.e
tcp_output_segment.e
tcp_output.e
tcp_output.s
tcp_output_segment.s
ip_output_if.s
dm9051_tx_.s ,len 1505 (LARGE-SIZE)
  (<TCP>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 00 33 08 00 45 00   05 d3 00 0d 00 00 ff 06  28 74 c0 a8 06 21 c0 a8
  (<TCP>) 00000020:  06 32 00 50 fc e2 00 00  47 0f c6 eb 92 68 50 19   15 37
 ..SrcIP 192.168.6.33  DestIP 192.168.6.50  Len 1505  (   80 -> 64738) flags 19
ip_output_if.e
tcp_output_segment.e
tcp_output.e
tcp_output.s
tcp_output.e
tcp_output.s
  (<TCP>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 00 33 08 00 45 00   00 28 00 0e 00 00 ff 06  2e 1e c0 a8 06 21 c0 a8
  (<TCP>) 00000020:  06 32 00 50 fc e2 00 00  4c bb c6 eb 92 69 50 10   15 36
 ..SrcIP 192.168.6.33  DestIP 192.168.6.50  Len 54  (   80 -> 64738) flags 10
tcp_output.e
tcp_output_segment.s
ip_output_if.s
  (<TCP>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 00 33 08 00 45 00   00 2c 00 0f 00 00 ff 06  2e 19 c0 a8 06 21 c0 a8
  (<TCP>) 00000020:  06 32 00 50 fc e3 00 00  19 70 95 1f 66 ed 60 12   16 d0
 ..SrcIP 192.168.6.33  DestIP 192.168.6.50  Len 58  (   80 -> 64739) flags 12
ip_output_if.e
tcp_output_segment.e
tcp_output.s
tcp_output.e
  (dm9 monitor <<rx  ) 00000000:  00 60 6e 00 00 33 08 97  98 c3 b9 a2 08 00 45 00   01 c4 ae dc 40 00 80 06  bc b3 c0 a8 06 32 c0 a8
  (dm9 monitor <<rx  ) 00000020:  06 21 fc e3 00 50 95 1f  66 ed 00 00 19 71 50 18   fa f0 bb f3 00 00 47 45  54 20 2f 66 61 76 69 63
  (dm9 monitor <<rx  ) 00000040:  6f 6e 2e 69 63 6f
 ..SrcIP 192.168.6.50  DestIP 192.168.6.33  Len 470  (64739 -> 80) flags 18 (PSH, ACK)
tcp_output.s
tcp_output_segment.s
ip_output_if.s
dm9051_tx_.s ,len 1514 (LARGE-SIZE)
  (<TCP>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 00 33 08 00 45 00   05 dc 00 10 00 00 ff 06  28 68 c0 a8 06 21 c0 a8
  (<TCP>) 00000020:  06 32 00 50 fc e3 00 00  19 71 95 1f 68 89 50 10   15 34
 ..SrcIP 192.168.6.33  DestIP 192.168.6.50  Len 1514  (   80 -> 64739) flags 10
ip_output_if.e
tcp_output_segment.e
tcp_output_segment.s
ip_output_if.s
dm9051_tx_.s ,len 1514 (LARGE-SIZE)
  (<TCP>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 00 33 08 00 45 00   05 dc 00 11 00 00 ff 06  28 67 c0 a8 06 21 c0 a8
  (<TCP>) 00000020:  06 32 00 50 fc e3 00 00  1f 25 95 1f 68 89 50 18   15 34
 ..SrcIP 192.168.6.33  DestIP 192.168.6.50  Len 1514  (   80 -> 64739) flags 18 (PSH, ACK)
ip_output_if.e
tcp_output_segment.e
tcp_output.e
tcp_output.s
tcp_output_segment.s
ip_output_if.s
dm9051_tx_.s ,len 1514 (LARGE-SIZE)
  (<TCP>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 00 33 08 00 45 00   05 dc 00 12 00 00 ff 06  28 66 c0 a8 06 21 c0 a8
  (<TCP>) 00000020:  06 32 00 50 fc e3 00 00  24 d9 95 1f 68 89 50 18   15 34
 ..SrcIP 192.168.6.33  DestIP 192.168.6.50  Len 1514  (   80 -> 64739) flags 18 (PSH, ACK)
ip_output_if.e
tcp_output_segment.e
tcp_output.e
tcp_output.s
tcp_output_segment.s
ip_output_if.s
  (<TCP>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 00 33 08 00 45 00   00 4e 00 13 00 00 ff 06  2d f3 c0 a8 06 21 c0 a8
  (<TCP>) 00000020:  06 32 00 50 fc e3 00 00  2a 8d 95 1f 68 89 50 19   15 34
 ..SrcIP 192.168.6.33  DestIP 192.168.6.50  Len 92  (   80 -> 64739) flags 19
ip_output_if.e
tcp_output_segment.e
tcp_output.e
tcp_output.s
tcp_output.e
tcp_output.s
  (<TCP>) 00000000:  08 97 98 c3 b9 a2 00 60  6e 00 00 33 08 00 45 00   00 28 00 14 00 00 ff 06  2e 18 c0 a8 06 21 c0 a8
  (<TCP>) 00000020:  06 32 00 50 fc e3 00 00  2a b4 95 1f 68 8a 50 10   15 33
 ..SrcIP 192.168.6.33  DestIP 192.168.6.50  Len 54  (   80 -> 64739) flags 10
tcp_output.e

