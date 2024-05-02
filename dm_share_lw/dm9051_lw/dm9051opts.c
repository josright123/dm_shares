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

//void dm9051_lock_arch(const char *project_name)
//{
//	const char *test1 = "freertos_demo TESTED";
//
//	printf("%s start: LWIP_TCPIP_CORE_LOCKING %d  /  LOCK_TCPIP_CORE()= %s\r\n", test1,
//			1, "sys_mutex_unlock(&lock_tcpip_core)");
//	printf("%s start: freeRTOS %d  / (TESTED GOOD, Must be sure that SPI lines are solid connection)\r\n", test1, 1);
//
//	printf("%s start: freeRTOS %d  / (TESTED GOOD, now, check \"freertos_tcpip_stack/port/lwipopts.h\")\r\n", project_name, freeRTOS);
//	bannerline_log();
//}
void dm9051_lock_arch_show(const char *project_name)
{
#if 0
	const char *test1 = "freertos_demo TESTED";

	printf("%s start: LWIP_TCPIP_CORE_LOCKING %d  /  LOCK_TCPIP_CORE()= %s\r\n", test1,
			1, "sys_mutex_unlock(&lock_tcpip_core)");
	printf("%s start: freeRTOS %d  / (TESTED GOOD, Must be sure that SPI lines are solid connection)\r\n", test1, 1);

	printf("%s start: freeRTOS %d  / (TESTED GOOD, now, check \"freertos_tcpip_stack/port/lwipopts.h\")\r\n", project_name, freeRTOS);
	bannerline_log();
#endif
}

#if 0
void GpioDisplay(void) {
  int i;
  for (i = 0; i < ETHERNET_COUNT; i++) {
	mstep_set_net_index(i);
	printf("@%s %s, %s, %s\r\n", mstep_conf_info(), mstep_conf_cpu_spi_ethernet(),
		mstep_conf_cpu_cs_ethernet(), mstep_spi_conf_name());
  }
}
#endif

void dm9051_opts_display(void)
{
#if 0
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
#endif
}

void dm9051_mac_ip_display(char *head)
{
  int b;
  uint8_t *pd;
  for (b = 0; b < ETHERNET_COUNT; b++) {
	  uint8_t buf[6];
	  mstep_set_net_index(b);
	  cspi_read_regs(DM9051_PAR, buf, 6, CS_EACH);
	  pd = mstep_eth_ip();
	  printf("%s.(chip-mac[%d]) %02x %02x %02x %02x %02x %02x  %d.%d.%d.%d\r\n",
		head, b, buf[0], buf[1], buf[2], buf[3], buf[4], buf[5],
		pd[0], pd[1], pd[2], pd[3]);
  }
}

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
