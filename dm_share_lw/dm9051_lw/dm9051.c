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

#if 0
#if 0
	#include <lwip/sys.h> //.temp
#else //0

	typedef   signed			char int8_t;
	typedef int8_t    			s8_t;

	/** Definitions for error constants. */
	//#if 1
	typedef enum {
	/** No error, everything OK. */
	  ERR_OK         = 0,
	/** Out of memory error.     */
	  ERR_MEM        = -1,
	/** Buffer error.            */
	  ERR_BUF        = -2,
	/** Timeout.                 */
	  ERR_TIMEOUT    = -3,
	/** Routing problem.         */
	  ERR_RTE        = -4,
	/** Operation in progress    */
	  ERR_INPROGRESS = -5,
	/** Illegal value.           */
	  ERR_VAL        = -6,
	/** Operation would block.   */
	  ERR_WOULDBLOCK = -7,
	/** Address in use.          */
	  ERR_USE        = -8,
	/** Already connecting.      */
	  ERR_ALREADY    = -9,
	/** Conn already established.*/
	  ERR_ISCONN     = -10,
	/** Not connected.           */
	  ERR_CONN       = -11,
	/** Low-level netif error    */
	  ERR_IF         = -12,

	/** Connection aborted.      */
	  ERR_ABRT       = -13,
	/** Connection reset.        */
	  ERR_RST        = -14,
	/** Connection closed.       */
	  ERR_CLSD       = -15,
	/** Illegal argument.        */
	  ERR_ARG        = -16
	} err_enum_t;

	/** Define LWIP_ERR_T in cc.h if you want to use
	 *  a different type for your platform (must be signed). */
	#ifdef LWIP_ERR_T
	typedef LWIP_ERR_T err_t;
	#else /* LWIP_ERR_T */
	typedef s8_t err_t;
	#endif /* LWIP_ERR_T*/
	//#endif

	#if NO_SYS
	#define sys_mutex_new(mu) ERR_OK
	#else /* NO_SYS */

	//typedef xSemaphoreHandle sys_mutex_t;

	/* Mutex functions: */

	/** Define LWIP_COMPAT_MUTEX if the port has no mutexes and binary semaphores
		should be used instead */
	#ifndef LWIP_COMPAT_MUTEX
	#define LWIP_COMPAT_MUTEX 0
	#endif

	#if LWIP_COMPAT_MUTEX
	#define sys_mutex_new(mutex)          sys_sem_new(mutex, 1)
	#else /* LWIP_COMPAT_MUTEX */
	err_t sys_mutex_new(sys_mutex_t *mutex);
	#endif /* LWIP_COMPAT_MUTEX */
	#endif /* NO_SYS */

#endif //0
#endif

#include "dm9051opts.h"
#include "dm9051_lw.h"
#include "dm9051_lw_debug.h"
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))

#if freeRTOS
	#include "lwip/sys.h"
	#include "lwip/err.h"
#endif

/* if "expression" is true, then execute "handler" expression */
#define DM9051_RX_RERXB(expression, handler) do { if ((expression)) { \
  handler;}} while(0)
#define DM9051_RX_BREAK(expression, handler) do { if ((expression)) { \
  handler;}} while(0)

#define	DM9051_Read_Rxb	DM9051_Read_Mem2X

//static
//uint16_t impl_dm9051_rx(uint8_t *buff);
uint16_t impl_dm9051_rx0(uint8_t *buff)
{
	u8 rxbyte, rx_status;
	u8 ReceiveData[4];
	u16 rx_len;

	#if TEST_PLAN_MODE
	u8 rxbyte_flg;
	int ipf = 0, udpf = 0, tcpf = 0;
	#endif

	#if 0
	/*if (!dm9051_is_flag_set(lw_flag, DM9051_FLAG_LINK_UP)) {
		if (unlink_count < UNLINK_COUNT_RESET) {
			unlink_count++;
		}
		if (unlink_count >= unlink_stamp) {*/
			#if 0
			/*uint16_t bmsr = dm9051_bmsr_update();
			printf("dm9051rx dm9051[%d](while bmsr %04x %s) step counter %04u\r\n",
				mstep_get_net_index(),
				bmsr, !dm9051_is_flag_set(lw_flag, DM9051_FLAG_LINK_UP) ? "PHY DOWN" : "PHY UP",
				unlink_stamp);*/
			#endif
			/*unlink_stamp = unlink_count + 1000;
		}
		return 0;
	}*/
	#endif

	rxbyte = DM9051_Read_Rxb(); //DM9051_Read_Reg(DM9051_MRCMDX);
	//DM9051_RXB_Basic(rxbyte); //(todo) Need sevice case.

	#if TEST_PLAN_MODE //TestMode.RX
	if (check_get() && !check_get_check_done()) { //(checkrxbcount > 1)
		if (rxbyte & RXB_ERR) { //(checksum_re_rxb(rxbyte) != 0x01 && checksum_re_rxb(rxbyte) != 0)
				check_decr_to_done();
				bannerline_log();
				printf(".(Err.rxb%2x)", rxbyte);
				if (check_get_check_done()) {
					printf("\r\n");
					printf("---------------------- on (Err.rxb == %2x) ,check_get_check_done() ----------------------\r\n", rxbyte);
					printf("-------------------------- DM9051_RX_RERXB will process follow -------------------------\r\n");
				}
		}
		else {
			if (rxbyte == 0) {

			  check_decr_to_done(); //need ?
			#if 0
			  printf(".(rxb%2x)", rxbyte);
			#endif
			  if (check_get_check_done()) {
				bannerline_log();
				printf("-------------------------- (rxb == 0) ,check_get_check_done() --------------------------\r\n");
				printf("-------------------------- On ChecksumOffload, DM9051_RX_RERXB will process follow -------------------------\r\n");
			  }
			}
			//else

			/*if (checksum_re_rxb(rxbyte) == DM9051_PKT_RDY)
			{
				//.printf("[rx %d]\r\n", get_testing_rx_count()); //testing_rx_count
				if (!tp_all_done()) {
					printf("[rx %d]\r\n", get_testing_rx_count());
					display_state();
				}
			}*/

			if (rxbyte != 0x01 && rxbyte != 0) {
				if (rxbyte == 0x40)
					;
				else {
					//Normal- case
					//printf(".ReceivePacket: rxb=%2x (processed) to %02x\r\n", rxbyte, rxbyte & 0x03);

					#if 1 //Extra- work around
					if (rxbyte & RCSSR_TCPS) {
						if (!(rxbyte & RCSSR_TCPP)) {
							rxbyte &= ~RCSSR_TCPS;
							bannerline_log();
							printf("Warn.case.ReceivePacket: rxb=%2x (step1) to %02x, mdra_ingress", rxbyte, rxbyte);
							rxrp_dump_print_init_show();
							bannerline_log();
						}
					}
					#endif
				}

				if (get_testplanlog(test_plan_log)) { //get_dm9051opts_testplanlog()
					if ((rxbyte & RCSSR_IPP)) {
						ipf = 1;
						if (!tp_all_done())
							printf("DM9RX: found IP, checksum %s\r\n", (rxbyte & RCSSR_IPS) ? "fail" : "ok");
					}
					if ((rxbyte & RCSSR_UDPP)) {
						udpf = 1;
						if (!tp_all_done())
							printf("DM9RX: found UDP, checksum %s\r\n", (rxbyte & RCSSR_UDPS) ? "fail" : "ok");
					}
					if ((rxbyte & RCSSR_TCPP)) {
						tcpf = 1;
						if (!tp_all_done())
							printf("DM9RX: found TCP, checksum %s\r\n", (rxbyte & RCSSR_TCPS) ? "fail" : "ok");
					}
				}
				rxbyte = checksum_re_rxb(rxbyte);
			}

			//.if (rxbyte != 0x01 && rxbyte != 0) {
			//.} else
			if (rxbyte == 1) {
			 #if 1
			  check_set_done(); //checkrxbcount = 1;
			  check_set_new(); //SET NEW AGAIN, we do continue testing...
			 #endif
			  //POST output, printf(".rxb%2x\r\n", rxbyte);
			}
		}
	}
	else
		DM9051_RX_RERXB((rxbyte != 0x01 && rxbyte != 0), rxbyte = checksum_re_rxb(rxbyte));
	#endif

	DM9051_RX_BREAK((rxbyte != 0x01 && rxbyte != 0), return ev_rxb(rxbyte));
	DM9051_RX_BREAK((rxbyte == 0), return 0);

	DM9051_Read_Mem(ReceiveData, 4);
	DM9051_Write_Reg(DM9051_ISR, 0x80);

	rx_status = ReceiveData[1];
	rx_len = ReceiveData[2] + (ReceiveData[3] << 8);

  //printf(" :drv.rx %02x %02x %02x %02x (len %u)\r\n",
  //		ReceiveData[0], ReceiveData[1], ReceiveData[2], ReceiveData[3], rx_len);

	#if 0
	if (rx_len != LEN64)
		printf(":drv.rx %02x %02x %02x %02x (len %u)\r\n",
			ReceiveData[0], ReceiveData[1], ReceiveData[2], ReceiveData[3], rx_len);
	#endif

	#if 0
	do {
		static int dump_odd_c = 0;
		if (rx_len & 1) {
		  printf("\r\n+ drv +oddfound %d (len %d)\r\n", ++dump_odd_c, rx_len);
		}
	} while(0);
	#endif

	DM9051_RX_BREAK((rx_status & 0xbf), return ev_status(rx_status)); //_err_hdlr("_dm9051f rx_status error : 0x%02x\r\n", rx_status, 0)
	DM9051_RX_BREAK((rx_len > RX_POOL_BUFSIZE), return impl_dm9051_err_hdlr("_dm9051f[%d] rx_len error : %u\r\n", PINCOD, rx_len, 0));

	DM9051_Read_Mem(buff, rx_len);
	DM9051_Write_Reg(DM9051_ISR, 0x80);

	if (!OPT_CONFIRM(tx_endbit)) //CH390
		dm_delay_ms(1);

	#if 0 	//test.
	dm9051_rxlog_monitor_rx(2, "Test <<rx  ", buff, rx_len); //HEAD_SPC
	#endif

	#if TEST_PLAN_MODE
	if (get_testplanlog(test_plan_log)) { //get_dm9051opts_testplanlog()
		if (ipf && !tp_all_done())
			printf("DM9RX: IP, checksum %02x %02x\r\n", buff[24], buff[25]);
		if (udpf && !tp_all_done())
			printf("DM9RX: UDP, checksum %02x %02x\r\n", buff[40], buff[41]);
		if (tcpf && !tp_all_done())
			printf("DM9RX: TCP, checksum %02x %02x\r\n", buff[50], buff[51]); //TBD

		rxbyte_flg = ReceiveData[0] & ~(0x03);

		if (!tp_all_done())
			printf("DM9RX[%d]: drv.rx %02x %02x %02x %02x (len %u), rxb %02x | %x\r\n",
				mstep_get_net_index(), ReceiveData[0], ReceiveData[1], ReceiveData[2], ReceiveData[3], rx_len, rxbyte_flg, rxbyte);
	}
	#endif

	/* Any after linkup */
	dm9051_link_to_hexdump(buff, rx_len);

	/* An assurence */
	if (dm9051_disp_and_check_rx(buff, rx_len)) { //ok. only 1st-pbuf
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
		uint16_t rwpa_w, mdra_ingress;

		dm9051_rx_unknow_pkt_inc_count();

		impl_read_rx_pointers(&rwpa_w, &mdra_ingress);
		printf("dm9051_disp_and_check_rx : Receive unit-cast UNKNOW pkt (err: %d) --------------- %04x / %04x\r\n",
			   DM9051_NUM_RXLOG_RST, rwpa_w, mdra_ingress); //or "0x%02x"

		hdlr_reset_process(mstep_eth_mac(), OPT_CONFIRM(hdlr_confrecv)); //CH390 opts //~return ev_rxb(rxbyte);
		
		impl_read_rx_pointers(&rwpa_w, &mdra_ingress);
		printf("  rwpa %04x / igrss %04x\r\n", rwpa_w, mdra_ingress);
		return 0;
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
	}
	return rx_len;
}

#define DM9051_TX_DELAY(expression, handler) do { if ((expression)) { \
  handler;}} while(0)

//static
void impl_dm9051_tx0(uint8_t *buf, uint16_t len)
{
	DM9051_Write_Reg(DM9051_TXPLL, len & 0xff);
	DM9051_Write_Reg(DM9051_TXPLH, (len >> 8) & 0xff);
	DM9051_Write_Mem(buf, len);
	DM9051_Write_Reg(DM9051_TCR, TCR_TXREQ); /* Cleared after TX complete */

	if (OPT_CONFIRM(tx_endbit))
		DM9051_TX_DELAY((DM9051_Read_Reg(DM9051_TCR) & TCR_TXREQ), dm_delay_us(5));
	else
		dm_delay_ms(1); //CH390

	dm9051_txlog_monitor_tx_all(2, buf, len); //_dm9051_txlog_monitor_tx
}

//static const uint8_t *impl_dm9051_init(const uint8_t *adr)
//{
//	if (dm9051_init_setup())
//		return hdlr_reset_process(_identify_eth_mac(adr), DM_TRUE);
//
//	return NULL;
//}

#define	PROJECT_NAME	"dm9051_driver_core"

//void dm9051_spi_configuration(int n)
void dm9051_boards_initialize(int n)
{
  DM_UNUSED_ARG(n);
  dm9051_board_counts(PROJECT_NAME); //printf("x2web start: [BOARD_SPI COUNT] %d  /  Operating: [ETHERNET COUNT] %d\r\n", BOARD_SPI_COUNT, ETHERNET_COUNT);
  dm9051_opts_display();
  printf("DM9051_DEBUGF-->dm9051_boards_initialize() ..\r\n"); //DM9051_DEBUGF(DM9051_LW_CONF,("DM9051_DEBUGF-->dm9051_boards_initialize() ..\r\n"));
  
  board_conf_configuration();
  dm9051_link_log_reset();
  
#if 0
  dm9051_opts_iomod_etc();
  dm9051_lock_arch_show("dm9051-demo");
#endif
}

const uint8_t *dm9051_init(const uint8_t *adr)
{
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	uint16_t id;
	const uint8_t *mac = NULL;
//	dm9051_lock_arch_show("dm9051-demo");

#if freeRTOS
	if (sys_mutex_new(&lock_dm9051_core) != ERR_OK) {
		printf("init, failed to create lock_dm9051_core\r\n");
	}
#endif /* freeRTOS */

	LOCK_TCPIP_COREx();
	//mac = impl_dm9051_init(adr); =
	//
	//= dm9051_init_setup(); //has, init.
	//= _identify_eth_mac(adr); //has, display_toset_mac();
	//= hdlr_reset_process(); //has, dm9051_core_reset(); ..dm9051_start(mac);
	//
	if (dm9051_init_setup(&id)) {
#if 1 //need so 'display'
		display_identity(mstep_spi_conf_name(), 0, NULL, 0, id, ".(Rst.process)"); //printf(".(Rst.process[%d])\r\n", mstep_get_net_index());
#endif
		mac = hdlr_reset_process(identify_eth_mac(adr, 0), DM_TRUE);
//		rx_pointer_show("dm9051_start");
//		rx_isr_show("dm9051_err_hdlr");
		rx_pointers_isr_show("dm9051_start");
	} else {
#if 1 //need so 'display'
		display_identity(mstep_spi_conf_name(), 0, NULL, 0, id, ""); //printf(".(Rst.process[%d])\r\n", mstep_get_net_index());
		printf(": dm9051_init_setup[%d] ::: FAIL ID %04x\r\n", mstep_get_net_index(), DM_GET_FIELD(uint16_t, read_chip_id));
#endif
	}
	
	ULOCK_TCPIP_COREx();
	return mac;
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

uint16_t dm9051_rx(uint8_t *buff)
{
	uint16_t len;
	LOCK_TCPIP_COREx();
	len = impl_dm9051_rx1(buff);
	ULOCK_TCPIP_COREx();
	return len;
}

uint32_t sys_now(void);
#define ICHK_FREQ_MS		725 //25 //250 //1500
#define ICHK_FREQ_MS_MIN	125

static int test_line7_ienter[ETHERNET_COUNT] = { 0 };

//uint16_t dm9051_rx_isr(uint8_t *buff)
//{
//#undef printf
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))

//	int xp = 0;
//	int pin;

//	uint8_t isr;
//	uint16_t len;
//	
//	LOCK_TCPIP_COREx();
//	
//	pin = mstep_get_net_index();
//	
//  isr = cspi_read_reg(DM9051_ISR);
//  DM9051_Write_Reg(DM9051_ISR, isr);

//	//if (isr & 1)
//	isr_local_time[pin] = sys_now() + ICHK_FREQ_MS;
//	
//	if ((isr & 1) && (test_line7_ienter[pin] < 3))
//		xp = 1;

//	if (xp)
//	  printf("INFO[%d]: line7() enter %d ... isr %02x\r\n", pin, ++test_line7_ienter[pin], isr);

//	len = impl_dm9051_rx1(buff);

//	if (xp) {
//	  isr = cspi_read_reg(DM9051_ISR);
//	  printf("INFO[%d]: line7() exit %d ... isr %02x\r\n", pin, test_line7_ienter[pin], isr);
//	}

//	ULOCK_TCPIP_COREx();
//	return len;
//#undef printf
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
//}

static uint32_t isr_local_time[ETHERNET_COUNT] = { 0 };

int dm9051_rx_isr_check(int pin)
{
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	int check = 0;
	uint8_t isr;
	if (sys_now() > isr_local_time[pin]) {
		LOCK_TCPIP_COREx();

		isr = DM9051_Read_Reg(DM9051_ISR);
		if (isr & 1) {
			printf("*INFO[%d]: dm9051_rx_isr_check(%d) enter ... isr %02x\r\n", pin, ++test_line7_ienter[pin], isr);  //"%d" 
			DM9051_Write_Reg(DM9051_ISR, isr);
			isr = DM9051_Read_Reg(DM9051_ISR);
			printf("*INFO[%d]: dm9051_rx_isr_check(%d) exit ... isr %02x\r\n", pin, test_line7_ienter[pin], isr);
			check = 1; // or rx_handler_direct(pin, FALSE/TRUE);
			isr_local_time[pin] = sys_now() + ICHK_FREQ_MS;
		}
		else
			isr_local_time[pin] = sys_now() + ICHK_FREQ_MS_MIN;

		ULOCK_TCPIP_COREx();
	}
	return check;
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

//void DEBUG_refresh_isr_check(void)
//{
//  int i;
//  for (i = 0; i< ETHERNET_COUNT; i++)
//	test_line7_ienter[i] = 0;
//}

void dm9051_rx_isr_clean(void)
{
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))

  static int test_line_ienter[ETHERNET_COUNT] = { 0 };
  int pin = mstep_get_net_index();
  int xp = 0;
  uint8_t isr;

  isr = DM9051_Read_Reg(DM9051_ISR);
	
	if ((isr & 1) && (test_line_ienter[pin] < 3))
	  xp = 1;

	if (xp)
	  printf("INP.a[%d]: extline enter %d ... isr %02x\r\n", pin, ++test_line_ienter[pin], isr);

  DM9051_Write_Reg(DM9051_ISR, isr);

	if (xp) {
		isr = DM9051_Read_Reg(DM9051_ISR);
		printf("INP.b[%d]: extline exit %d ... isr %02x\r\n", pin, test_line_ienter[pin], isr);
	}

#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

void dm9051_tx(uint8_t *buf, uint16_t len)
{
	LOCK_TCPIP_COREx();
	impl_dm9051_tx1(buf, len);
	ULOCK_TCPIP_COREx();
}

uint16_t dm9051_read_chip_id(void)
{
	uint16_t id;
	LOCK_TCPIP_COREx();
	id = impl_read_chip_id();
	ULOCK_TCPIP_COREx();
	return id;
}

void dm9051_read_rx_pointers(u16 *rwpa_wt, u16 *mdra_rd) {
	LOCK_TCPIP_COREx();
	impl_read_rx_pointers(rwpa_wt, mdra_rd);
	ULOCK_TCPIP_COREx();
}

uint16_t dm9051_bmsr_update(void)
{
  uint16_t val;
  LOCK_TCPIP_COREx();
  val = impl_phy_read(PHY_STATUS_REG);
  if (!(val & 0x4))
	dm9051_link_log_reset();
  ULOCK_TCPIP_COREx();
  return val;
}

uint16_t dm9051_err_hdlr(char *errstr, u32 invalue, u8 zerochk)
{
	u16 ret;
	LOCK_TCPIP_COREx();
	ret = impl_dm9051_err_hdlr(errstr, PINCOD, invalue, zerochk);
	ULOCK_TCPIP_COREx();
	return ret;
}
