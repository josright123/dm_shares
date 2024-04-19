//
// dm9051opts.c
//
#if 0 //only for Lwip
#include <lwip/sys.h> //.temp
#endif
#include "dm9051opts.h"
#include "dm9051_lw.h"

u8 gfirst_log[ETHERNET_COUNT];

void dm9051_board_counts(const char *project_name)
{
	bannerline_log();
	bannerline_log();
	printf("%s start: [BOARD_SPI COUNT] %d  /  Operating: [ETHERNET COUNT] %d\r\n", project_name, BOARD_SPI_COUNT, ETHERNET_COUNT);
	bannerline_log();
}

void dm9051_lock_arch(const char *project_name)
{
	const char *test1 = "freertos_demo TESTED";

	printf("%s start: LWIP_TCPIP_CORE_LOCKING %d  /  LOCK_TCPIP_CORE()= %s\r\n", test1,
			1, "sys_mutex_unlock(&lock_tcpip_core)");
	printf("%s start: freeRTOS %d  / (TESTED GOOD, Must be sure that SPI lines are solid connection)\r\n", test1, 1);
//#if LWIP_TCPIP_CORE_LOCKING//#else//#endif
//#if freeRTOS//#endif
#if 0 //only for Lwip
	printf("%s start: LWIP_TCPIP_CORE_LOCKING %d  /  LOCK_TCPIP_CORE()= %s\r\n", project_name,
			LWIP_TCPIP_CORE_LOCKING, "(none)");
#endif
	printf("%s start: freeRTOS %d  / (TESTED GOOD, now, check \"freertos_tcpip_stack/port/lwipopts.h\")\r\n", project_name, freeRTOS);
	bannerline_log();
}

void GpioDisplay(void) {
  int i;
  for (i = 0; i < ETHERNET_COUNT; i++) {
	mstep_set_net_index(i);
	printf("@%s %s, %s, %s\r\n", mstep_conf_info(), mstep_conf_cpu_spi_ethernet(),
		mstep_conf_cpu_cs_ethernet(), mstep_spi_conf_name());
  }
}

void dm9051_opts_display(void)
{
	int i;
	GpioDisplay();
	for (i = 0; i< ETHERNET_COUNT; i++) {
		mstep_set_net_index(i);
		bannerline_log();
		printf("dm9051[%d]_options_display:\r\n", mstep_get_net_index());
		printf(" - core rst mode, %s\r\n", DM_GET_DESC(enable_t, generic_core_rst)); //dm9051opts_descgeneric_core_rst()
		printf(" - tx_endbit mode, %s\r\n", DM_GET_DESC(enable_t, tx_endbit)); //dm9051opts_desctx_endbit()
		bannerline_log();
		//..
	}
}

#if DM9051OPTS_LOG_ENABLE
	void dm9051_opts_iomod_etc(void)
	{
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
		
	#if TO_ADD_CODE_LATER_BACK
		for (i = 0; i< ETHERNET_COUNT; i++) {
			uint8_t *macaddr;
			mstep_set_net_index(i);
			macaddr = mstep_eth_mac();
			printf("config tobe mac[%d] %02x%02x%02x%02x%02x%02x\r\n", i, macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
		}
	#endif
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
	}

	void ethcnt_ifdiplay(void)
	{
	#if 0
		int i;
		for (i = 0; i< _ETHERNET_COUNT; i++) {
			mstep_set_net_index(i);
			NetifDisplay(i);
			lEepromDisplay(i);
		}
	#endif
	}
#endif

void first_log_init(void)
{
	int i;
	for (i = 0; i < ETHERNET_COUNT; i++)
		gfirst_log[i] = 1; //Can know the first time reset the dm9051 chip!
}

u8 first_log_get(int i)
{
	u8 if_log = 
		gfirst_log[i];
	gfirst_log[i] = 0; //first_log_clear(i);
	return if_log;
}

//void first_log_clear(int i)
//{
//	gfirst_log[i] = 0;
//}

/* , uint8_t trans_type
 * == MULTI_TRANS,
 *  return : found id number
 */
//int TRANS_DUAL(trn_conn_t f) {
//  int i;
//  int nID = 0;
//  uint16_t id;
//  for (i = 0; i < ETHERNET_COUNT; i++) {
	//.mstep_set_net_index(i); + //dm9051_init(_mstep_eth_mac()); 
//	id = f(i); //= drviver_init(i)
//	if (check_chip_id(id))
//	 nID++;
//  }
//return nID;
//}

//voidfn_dual
void ETH_COUNT_VOIDFN(voidpin_t f) {
  int i;
  for (i = 0; i < ETHERNET_COUNT; i++) {
	  mstep_set_net_index(i);
	  f(i);
  }
}

//voidtx_dual
void ETH_COUNT_VOIDTX(voidtx_t pinfunc, uint8_t *buf, uint16_t len) {
  int i;
  for (i = 0; i < ETHERNET_COUNT; i++) {
	  mstep_set_net_index(i);
	  pinfunc(i, buf, len);
  }
}
