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
#include "dm9051opts.h"
#include "dm9051_lw.h"
#include "dm9051_lw_conf.h"
#include "dm9051_cboard.h"

#include "dm9051_lw_cspi.h"
#include "dm9051_rtos.h" //=dm9051_rtos.h
//#include "dm9051_lw_debug.h"
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
#include "../freertos_tasks_debug.h"
#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_NETCONF_ON, /*SEMA_OFF*/ SEMA_ON, "[L]", (fmt, ##__VA_ARGS__))

#if freeRTOS
xSemaphoreHandle lock_dm9051_core;
#endif /* freeRTOS */

const uint8_t *dm9051_init(const uint8_t *adr)
{
  DM9051_BOARD_CFG((driver_config_board), "device_driver_config_board\r\n", cboard_conf_configuration()); //device_driver config board
  DM9051_MUTEX_CFG((freeRTOS), "create freertos mutex\r\n", lock_dm9051_core = mutex_create_start());

  impl_reset_pulse();

  do {
	  const uint8_t *mac;
	  DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core)); //LOCK_TCPIP_COREx();
	  mac = impl_dm9051_init(adr);
	  DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core)); //ULOCK_TCPIP_COREx();
	  return mac;
  } while(0);
#if 0
  //return NULL;
#endif
}

uint16_t dm9051_rx(uint8_t *buff)
{
	uint16_t len;
	DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core)); //LOCK_TCPIP_COREx();
	len = impl_dm9051_rx1(buff);
	DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core)); //ULOCK_TCPIP_COREx();
	return len;
#if 0
	//return 0;
#endif
}

void dm9051_tx(uint8_t *buf, uint16_t len)
{
	DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core)); //LOCK_TCPIP_COREx();
	impl_dm9051_tx1(buf, len);
	DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core)); //ULOCK_TCPIP_COREx();
#if 0
#endif
}

uint16_t dm9051_isr_disab(void) {
	uint16_t isrs;
	DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core)); //LOCK_TCPIP_COREx();
	isrs = impl_dm9051_isr_disab();
	DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core)); //ULOCK_TCPIP_COREx();
	return isrs;
}

uint16_t dm9051_isr_enab(void)
{
	uint16_t isrs;
	DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core)); //LOCK_TCPIP_COREx();
	isrs = impl_dm9051_isr_enab();
	DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core)); //ULOCK_TCPIP_COREx();
	return isrs;
}

void dm9051_mcu_irq_disab(void)
{
	impl_disable_mcu_irq(); //_dm9051_eth_irq_disab(IRQn_Type irqn); //dm9051_disable_mcu_irq()
}
void dm9051_mcu_irq_enab(void)
{
	impl_enable_mcu_irq(); //dm9051_eth_irq_enab(IRQn_Type irqn, nvic_priority_group_type priority); //dm9051_enable_mcu_irq()
}

#if 1 //only lwip
#if 0
//#include "lwip/opt.h"
//#include "lwip/def.h"
//#include "lwip/mem.h"
//#include "lwip/pbuf.h"
//#include "lwip/sys.h"
//#include <lwip/stats.h>
//#include <lwip/snmp.h>
//#include "netif/etharp.h"
//#include "netif/ppp/pppoe.h"
//#include "lwip/err.h"
//#include "lwip/tcpip.h"
//void check_link_up_can_fix(struct netif *netif, uint32_t id_bmsr) {
//}
#endif
#endif

uint16_t dm9051_read_chip_id(void) {
	uint16_t id;
	DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core)); //LOCK_TCPIP_COREx();
	id = impl_read_chip_id();
	DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core)); //ULOCK_TCPIP_COREx();
	return id;
	//return 0x9051;
}
uint16_t dm9051_bmsr_update(void) {
	uint16_t val;
	DM9051_MUTEX_OPS((freeRTOS), sys_mutex_lock_start(&lock_dm9051_core)); //LOCK_TCPIP_COREx();
	val = impl_phy_read(PHY_STATUS_REG);
	DM9051_MUTEX_OPS((freeRTOS), sys_mutex_unlock_end(&lock_dm9051_core)); //ULOCK_TCPIP_COREx();
	return val;
	//return 0x786d;
}

//void dm9051_read_rx_pointers(uint16_t *rwpa_wt, uint16_t *mdra_rd) {
//	LOCK_TCPIP_COREx();
//	impl_read_rx_pointers(rwpa_wt, mdra_rd);
//	ULOCK_TCPIP_COREx();
//}

//uint16_t dm9051_bmsr_update(void) {
//  uint16_t val;
//  LOCK_TCPIP_COREx();
//  val = impl_phy_read(PHY_STATUS_REG);
//  if (!(val & 0x4))
//		dm9051_link_log_reset();
//  ULOCK_TCPIP_COREx();
//  return val;
//}

//uint16_t dm9051_err_hdlr(char *errstr, u32 invalue, u8 zerochk) {
//	uint16_t ret;
//	LOCK_TCPIP_COREx();
//	ret = impl_dm9051_err_hdlr(errstr, PINCOD, invalue, zerochk);
//	ULOCK_TCPIP_COREx();
//	return ret;
//}

//impl_functions

void impl_reset_pulse(void)
{
	dm9051_pwron_reset();
}

const uint8_t *impl_dm9051_init(const uint8_t *adr)
{
//#undef printf
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	uint16_t id;
	const uint8_t *mac = NULL;

	if (dm9051_init_setup(&id)) {
		const uint8_t *macaddr;
		display_identity("SPI", 0, NULL, 0, id, ".(Rst.process)");
		macaddr = identify_eth_mac(adr, 0);
		mac = hdlr_reset_process(macaddr, DM_TRUE);
	} 
	//.printf(": dm9051_init_setup[%d] ::: ChipID %04x\r\n", 0, identified_chip_id());
	return mac;
//#undef printf
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

//rx
uint16_t impl_dm9051_rx1(uint8_t *buff)
{
	return buff_rx(buff);
}

//tx
void impl_dm9051_tx1(uint8_t *buf, uint16_t len)
{
	buff_tx(buf, len);
}
