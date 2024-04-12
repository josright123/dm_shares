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
#include "dm9051opts.h" //TRANS_CONN/mstep_eth_mac/mstep_get_net_index
#include "dm9051_lw.h" //dm9051_mac_adr/dm9051_init/check_chip_id/dm9051_tx
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
//  error_status status;
//  emac_nvic_configuration();
  /* emac periph clock enable */
//  crm_periph_clock_enable(CRM_EMAC_PERIPH_CLOCK, TRUE);
//  crm_periph_clock_enable(CRM_EMACTX_PERIPH_CLOCK, TRUE);
//  crm_periph_clock_enable(CRM_EMACRX_PERIPH_CLOCK, TRUE);
//  emac_pins_configuration();
//  status = emac_layer2_configuration();
//  return status;
  dm9051_boards_initialize(ETHERNET_COUNT); //at32_dm9051_initialize();
  dm9051_opts_iomod_etc();

  return SUCCESS;
}

/**
  * @brief  configures emac irq channel.
  * @param  none
  * @retval none
  */
//void emac_nvic_configuration(void)
//{
//  nvic_irq_enable(EMAC_IRQn, 4, 0);
//}

/**
  * @brief  configures emac required pins.
  * @param  none
  * @retval none
  */
//void emac_pins_configuration(void)
//{
//  gpio_init_type gpio_init_struct = {0};
//  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
//  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
//  crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
//  crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
//  crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK, TRUE);
//  crm_periph_clock_enable(CRM_GPIOG_PERIPH_CLOCK, TRUE);
//  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE2, GPIO_MUX_11);
//  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
//  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
//  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
//  gpio_init_struct.gpio_pins = GPIO_PINS_2;
//  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
//  gpio_init(GPIOA, &gpio_init_struct);
//  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE1, GPIO_MUX_11);
//  gpio_init_struct.gpio_pins = GPIO_PINS_1;
//  gpio_init(GPIOC, &gpio_init_struct);

  #ifdef MII_MODE
//  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE8, GPIO_MUX_11);
//  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE11, GPIO_MUX_11);
//  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE12, GPIO_MUX_11);
//  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE13, GPIO_MUX_11);
//  gpio_init_struct.gpio_pins = GPIO_PINS_8 | GPIO_PINS_11 | GPIO_PINS_12 | GPIO_PINS_13;
//  gpio_init(GPIOB, &gpio_init_struct);
//  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE2, GPIO_MUX_11);
//  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE3, GPIO_MUX_11);
//  gpio_init_struct.gpio_pins = GPIO_PINS_2 | GPIO_PINS_3;
//  gpio_init(GPIOC, &gpio_init_struct);
//  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE8, GPIO_MUX_11);
//  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE9, GPIO_MUX_11);
//  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE10, GPIO_MUX_11);
//  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE11, GPIO_MUX_11);
//  gpio_pin_mux_config(GPIOD, GPIO_PINS_SOURCE12, GPIO_MUX_11);
//  gpio_init_struct.gpio_pins = GPIO_PINS_8 | GPIO_PINS_9 | GPIO_PINS_10 | GPIO_PINS_11 | GPIO_PINS_12;
//  gpio_init(GPIOD, &gpio_init_struct);
//  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE0, GPIO_MUX_11);
//  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE1, GPIO_MUX_11);
//  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE3, GPIO_MUX_11);
//  gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1 | GPIO_PINS_3;
//  gpio_init(GPIOA, &gpio_init_struct);
//
//  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE10, GPIO_MUX_11);
//  gpio_init_struct.gpio_pins = GPIO_PINS_10;
//  gpio_init(GPIOB, &gpio_init_struct);
  #endif

//  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE1, GPIO_MUX_11);
//  gpio_init_struct.gpio_pins = GPIO_PINS_1;
//  gpio_init(GPIOA, &gpio_init_struct);
//}

/**
  * @brief  configures emac layer2
  * @param  none
  * @retval error or success
  */
//error_status emac_layer2_configuration(void)
//{
//  emac_dma_config_type dma_control_para;
//  crm_periph_clock_enable(CRM_SCFG_PERIPH_CLOCK, TRUE);
//  scfg_emac_interface_set(SCFG_EMAC_SELECT_MII);

//  crm_clock_out1_set(CRM_CLKOUT1_PLL);
//  crm_clkout_div_set(CRM_CLKOUT_INDEX_1, CRM_CLKOUT_DIV1_5, CRM_CLKOUT_DIV2_2);
//
//  /* reset phy */
//  reset_phy();
//  /* reset emac ahb bus */
//  emac_reset();
//
//  /* software reset emac dma */
//  emac_dma_software_reset_set();
//
//  while(emac_dma_software_reset_get() == SET);
//
//  emac_control_para_init(&mac_control_para);
//
//  mac_control_para.auto_nego = EMAC_AUTO_NEGOTIATION_ON;
//
//  if(emac_phy_init(&mac_control_para) == ERROR)
//  {
//    return ERROR;
//  }
//
//  emac_dma_para_init(&dma_control_para);
//
//  dma_control_para.rsf_enable = TRUE;
//  dma_control_para.tsf_enable = TRUE;
//  dma_control_para.osf_enable = TRUE;
//  dma_control_para.aab_enable = TRUE;
//  dma_control_para.usp_enable = TRUE;
//  dma_control_para.fb_enable = TRUE;
//  dma_control_para.flush_rx_disable = TRUE;
//  dma_control_para.rx_dma_pal = EMAC_DMA_PBL_32;
//  dma_control_para.tx_dma_pal = EMAC_DMA_PBL_32;
//  dma_control_para.priority_ratio = EMAC_DMA_1_RX_1_TX;
//
//  emac_dma_config(&dma_control_para);
//  emac_dma_interrupt_enable(EMAC_DMA_INTERRUPT_NORMAL_SUMMARY, TRUE);
//  emac_dma_interrupt_enable(EMAC_DMA_INTERRUPT_RX, TRUE);
//
//  return SUCCESS;
//}

/**
  * @brief  reset layer 1
  * @param  none
  * @retval none
  */
//void static reset_phy(void)
//{
//}

/**
  * @brief  initialize emac phy
  * @param  none
  * @retval SUCCESS or ERROR
  */
//error_status emac_phy_init(emac_control_config_type *control_para)
//{
//  emac_clock_range_set();
//  if(emac_phy_register_reset() == ERROR)
//  {
//    return ERROR;
//  }
//  emac_control_config(control_para);
//  return SUCCESS;
//}

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
  #if 0
  //dm no need!
	//    emac_speed_config(mac_control_para.auto_nego, mac_control_para.duplex_mode, mac_control_para.fast_ethernet_speed);
	//    vTaskDelay(300);
	//    emac_start();
  #endif
  }
  else
  {
  #if 0
  //dm no need!
	//    emac_stop();
  #endif
  }

  ethernetif_notify_conn_changed(netif);
}

/**
  * @}
  */

/**
  * @}
  */
