/**
  **************************************************************************
  * @file     netconf.c
  * @version  v2.0.0
  * @date     2020-11-02
  * @brief    network connection configuration
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

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "lwip/memp.h"
#include "lwip/tcp.h"
#include "lwip/priv/tcp_priv.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "ethernetif.h"
#include "netconf.h"
#include "ethernetif.h"
#include "lwip/tcpip.h"
#include "dm9051opts.h"
#include "at32_dm.h"

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define MAC_ADDR_LENGTH                  (6)
#define ADDR_LENGTH                      (4)
/* Private variables ---------------------------------------------------------*/
struct netif netif[ETHERNET_COUNT];
// struct netif netif2;
//int spipin0 = 1;

TaskHandle_t link_status_handler;
volatile uint32_t tcp_timer = 0;
volatile uint32_t arp_timer = 0;

static uint8_t mac_address[MAC_ADDR_LENGTH] = {0x00, 0x60, 0x6E, 0x00, 0x01, 0x17};
// static uint8_t mac_address[MAC_ADDR_LENGTH] = {0, 0, 0x44, 0x45, 0x56, 1};
#if LWIP_DHCP
volatile uint32_t dhcp_fine_timer = 0;
volatile uint32_t dhcp_coarse_timer = 0;
#else
static uint8_t local_ip[ADDR_LENGTH]   = {192, 168, 249,  81};
static uint8_t local_gw[ADDR_LENGTH]   = {192, 168, 249,   1};
static uint8_t local_mask[ADDR_LENGTH] = {255, 255, 255, 0};
#endif

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  initializes the lwip stack
  * @param  none
  * @retval none
  */
void tcpip_stack_init_2(int spino)
{
	struct ethernetif *ethernetif;
	uint8_t *pd;
  ip_addr_t ipaddr2;
  ip_addr_t netmask2;
  ip_addr_t gw2;

#if LWIP_DHCP  //need DHCP server
  ipaddr2.addr = 0;
  netmask2.addr = 0;
  gw2.addr = 0;

#else
  pd = identify_tcpip_ip(NULL, spino);
  IP4_ADDR(&ipaddr2, pd[0], pd[1], pd[2], pd[3]);
  pd = identify_tcpip_gw(NULL, spino);
  IP4_ADDR(&gw2, pd[0], pd[1], pd[2], pd[3]);
  pd = identify_tcpip_mask(NULL, spino);
  IP4_ADDR(&netmask2, pd[0], pd[1], pd[2], pd[3]);
#endif

  pd = identify_eth_mac(NULL, spino);
  lwip_set_mac_address(pd, spino);
//	netif[spino].spino = spino;

	ethernetif = mem_malloc(sizeof(struct ethernetif));
  if (ethernetif == NULL)
  {
    LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_init: out of memory\n"));
    return ;
  }
  ethernetif->ethaddr = (struct eth_addr *)&(netif[spino].hwaddr[0]);
	ethernetif->spino = spino;
//	netif[spino].state = ethernetif;
  if(netif_add(&netif[spino], &ipaddr2, &netmask2, &gw2, ethernetif, &ethernetif_init, &tcpip_input) == NULL)
  {
    while(1);
  }

  printf("netif_add.e %u.%u.%u.%u ............ netif_add( ip...) done ............\r\n",
		ip4_addr1_16(netif_ip4_addr(&netif[spino])),
		ip4_addr2_16(netif_ip4_addr(&netif[spino])),
		ip4_addr3_16(netif_ip4_addr(&netif[spino])),
		ip4_addr4_16(netif_ip4_addr(&netif[spino])));

  netif_set_default(&netif[spino]);

#if LWIP_DHCP
  dhcp_start(&netif[spino]);
#endif

  netif_set_up(&netif[spino]);
  netif_set_link_callback(&netif[spino], ethernetif_update_config);
  xTaskCreate((TaskFunction_t)ethernetif_set_link, "ethernetif_set_link", 64, &netif[spino], tskIDLE_PRIORITY + 2, &link_status_handler);
}


/**
  * @brief  called when a frame is received
  * @param  none
  * @retval none
  */
void lwip_pkt_handle(void)
{
  /* Read a received packet from the Ethernet buffers and send it to the lwIP for handling */
	for(int i = 0; i < ETHERNET_COUNT; i++)
	{
		if(netif[i].next == NULL)
    {
        return; // netif[i] does not exist, return from the function
    }
		if(ethernetif_input(&netif[i]) != ERR_OK)
		{
			//    while(1);
		}
	}
}

/**
  * @brief  lwip periodic tasks
  * @param  localtime the current localtime value
  * @retval none
  */
void lwip_periodic_handle(volatile uint32_t localtime)
{

  /* TCP periodic process every 250 ms */
  if (localtime - tcp_timer >= TCP_TMR_INTERVAL)
  {
    tcp_timer =  localtime;
    tcp_tmr();
  }
  /* ARP periodic process every 5s */
  if (localtime - arp_timer >= ARP_TMR_INTERVAL)
  {
    arp_timer =  localtime;
    etharp_tmr();
  }

#if LWIP_DHCP
  /* Fine DHCP periodic process every 500ms */
  if (localtime - dhcp_fine_timer >= DHCP_FINE_TIMER_MSECS)
  {
    dhcp_fine_timer =  localtime;
    dhcp_fine_tmr();
  }
  /* DHCP Coarse periodic process every 60s */
  if (localtime - dhcp_coarse_timer >= DHCP_COARSE_TIMER_MSECS)
  {
    dhcp_coarse_timer =  localtime;
    dhcp_coarse_tmr();
  }
#endif
}
