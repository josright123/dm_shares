/**
  **************************************************************************
  * @file     at32_emac.c
  * @version  v2.0.6
  * @date     2022-03-11
  * @brief    emac config program
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

/* includes ------------------------------------------------------------------*/
// #include "at32f403a_407_board.h"
#include "lwip/dhcp.h"
#include "FreeRTOS.h"
#include "task.h"
#include "dm9051opts.h"
#include "dm9051_lw.h"
#include "at32_dm.h"
#include "lwip/tcpip.h"
#include "ethernetif.h"

#include "../freertos_tasks_debug.h"
#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_AT32_DM, SEMA_OFF, "[xx]", (fmt, ##__VA_ARGS__))

/** @addtogroup AT32F437_periph_examples
  * @{
  */

/** @addtogroup 437_EMAC_tcp_server
  * @{
  */

emac_control_config_type mac_control_para;

/**
  * @brief  enable emac clock and gpio clock
  * @param  none
  * @retval success or error
  */
error_status emac_system_init(void)
{
  dm9051_boards_initialize(ETHERNET_COUNT); //at32_dm9051_initialize();
  dm9051_opts_iomod_etc();

  return SUCCESS;
}

/**
  * @brief  updates the link states
  * @param  none
  * @retval link state 0: disconnect, 1: connection
  */
uint16_t link_update(pin)
{
  uint16_t bmsr = dm9051_bmsr_update(pin);    // pin = 0
  return bmsr & 0x0004 ? 1 : 0;
}

/**
  * @brief  this function sets the netif link status.
  * @param  netif: the network interface
  * @retval none
  */
void ethernetif_set_link(void const *argument)
{
  uint16_t regvalue = 0;
  struct netif *netif = (struct netif *)argument;
	struct ethernetif *ethernetif = netif->state;
	int spino = ethernetif->spino;		// pin = 0
	
  for(;;)
  {
    LOCK_TCPIP_CORE();
    /* Read PHY_BSR*/
    // regvalue = link_update(spino);
		regvalue = dm9051_link_update(spino);		// pin = 0 , spipin0 = 1
    // printf("link status: 0x%0X\r\n", regvalue);
    UNLOCK_TCPIP_CORE();

    if(regvalue > 0)
    {
      at32_led_on(LED4);
      at32_led_off(LED2);
    }
    else
    {
      at32_led_on(LED2);
      at32_led_off(LED4);
    }
    /* Check whether the netif link down and the PHY link is up */
    if(!netif_is_link_up(netif) && (regvalue))
    {
      /* network cable is connected */
      netif_set_link_up(netif);
    }
    else if(netif_is_link_up(netif) && (!regvalue))
    {
      /* network cable is dis-connected */
      netif_set_link_down(netif);
    }

    /* Suspend thread for 200 ms */
    vTaskDelay(200);
  }
}

/**
  * @brief  this function notify user about link status changement.
  * @param  netif: the network interface
  * @retval none
  */
static void ethernetif_notify_conn_changed(struct netif *netif)
{
  /* note : this is function could be implemented in user file
            when the callback is needed,
  */

  if (netif_is_link_up(netif)) {
    netif_set_up(netif);

#if LWIP_DHCP
    /*  creates a new dhcp client for this interface on the first call.
    note: you must call dhcp_fine_tmr() and dhcp_coarse_tmr() at
    the predefined regular intervals after starting the client.
    you can peek in the netif->dhcp struct for the actual dhcp status.*/
    dhcp_start(netif);
#endif
  }
  else
    netif_set_down(netif);
}

/**
  * @brief  link callback function, this function is called on change of link status
  *         to update low level driver configuration.
  * @param  netif: the network interface
  * @retval none
  */
void ethernetif_update_config(struct netif *netif)
{
  if(netif_is_link_up(netif))
  {

  }
  ethernetif_notify_conn_changed(netif);
}

/**
  * @}
  */

/**
  * @}
  */
