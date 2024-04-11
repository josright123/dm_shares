/**
 * @file
 * Ethernet Interface Skeleton
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
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
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/*
 * This file is a skeleton for developing Ethernet network interface
 * drivers for lwIP. Add code to the low_level functions and do a
 * search-and-replace for the word "ethernetif" to replace it with
 * something that better describes your network interface.
 */

#include <string.h>
#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include "netif/etharp.h"
#include "netif/ppp/pppoe.h"
#include "lwip/err.h"
#include "ethernetif.h"

// #include "at32f435_437_emac.h"
// #include "at32f403a_407_emac.h"
#include "dm9051opts.h" //TRANS_CONN/mstep_eth_mac/mstep_get_net_index
#include "dm9051_lw.h" //dm9051_mac_adr/dm9051_init/check_chip_id/dm9051_tx
#include "lwip/tcpip.h"

int spipin0 = 0;  	// pin = 0
extern struct netif netif;
//struct netif *lwip_netif= &netif;

/* TCP and ARP timeouts */
//volatile int tcp_end_time, arp_end_time;

/* Define those to better describe your network interface. */
#define IFNAME0 'a'
#define IFNAME1 't'

#define OVERSIZE_LEN			PBUF_POOL_BUFSIZE //#define _PBUF_POOL_BUFSIZE 1514 //defined in "lwipopts.h" (JJ20201006)
#define RXBUFF_OVERSIZE_LEN		(OVERSIZE_LEN+2)
union {
	uint8_t rx;
	uint8_t tx;
} EthBuff[RXBUFF_OVERSIZE_LEN]; //[Single Task project.] not occupied by concurrently used.

/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct ethernetif
{
  struct eth_addr *ethaddr;
  /* Add whatever per-interface state that is needed here. */
  int unused;
};

/* Forward declarations. */
err_t  ethernetif_input(struct netif *netif);

#define EMAC_RXBUFNB        10
#define EMAC_TXBUFNB        10

uint8_t MACaddr[6];
//emac_dma_desc_type  DMARxDscrTab[EMAC_RXBUFNB], DMATxDscrTab[EMAC_TXBUFNB];
//uint8_t Rx_Buff[EMAC_RXBUFNB][EMAC_MAX_PACKET_LENGTH], Tx_Buff[EMAC_TXBUFNB][EMAC_MAX_PACKET_LENGTH];

/**
 * Setting the MAC address.
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
void lwip_set_mac_address(uint8_t* macadd, int pin)
{
  MACaddr[0] = macadd[0];
  MACaddr[1] = macadd[1];
  MACaddr[2] = macadd[2];
  MACaddr[3] = macadd[3];
  MACaddr[4] = macadd[4];
  MACaddr[5] = macadd[5];

  //emac_local_address_set(macadd);
  dm9051_mac_adr((const uint8_t *)macadd, pin); // pin = 0
}

int n_verify_id = 0;

//static uint16_t drviver_init(int pin) {
//	printf("drviver_init(pin = %d)\r\n", pin);
//	mstep_set_net_index(pin);
//	return dm9051_init(mstep_eth_mac()); //driver set mac
//}
static uint16_t drviver_init_nondual(int pin) {
  mstep_set_net_index(pin);       // Default pin = 0
	printf("drviver_init(pin = %d)\r\n", pin);
	return dm9051_init(mstep_eth_mac(pin), pin); //driver set mac, // pin = 0
}

//void dm9051_init_dual(void) {
//	n_verify_id = TRANS_DUAL(drviver_init);
//}

void dm9051_init_nondual(int pin) {
	uint16_t id = drviver_init_nondual(pin); //TRANS_NONDUAL(drviver_init_nondual);
	if (check_chip_id(id))
	 n_verify_id++;
}

void dm9051_tx_dual(uint8_t *buf, uint16_t len, int pin)
{
	mstep_set_net_index(pin);
	dm9051_txlog_monitor_tx_all(2, buf, len); //_dm9051_txlog_monitor_tx
	dm9051_tx(buf, len, pin);     // pin = 0
}


/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void
low_level_init(struct netif *netif)
{
  /* set MAC hardware address length */
  netif->hwaddr_len = ETHARP_HWADDR_LEN;

  /* set MAC hardware address */
  netif->hwaddr[0] =  MACaddr[0];
  netif->hwaddr[1] =  MACaddr[1];
  netif->hwaddr[2] =  MACaddr[2];
  netif->hwaddr[3] =  MACaddr[3];
  netif->hwaddr[4] =  MACaddr[4];
  netif->hwaddr[5] =  MACaddr[5];

  /* maximum transfer unit */
  netif->mtu = 1500;

  /* device capabilities */
  /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

  /* Enable MAC and DMA transmission and reception */
  //emac_start();
  dm9051_init_nondual(spipin0); //_dm9051_init(MACaddr);
  //dm9051_init_dual();
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
static err_t
low_level_output(struct netif *netif, struct pbuf *p)
{
  struct pbuf *q;
  int l = 0;

  uint8_t *buffer = &EthBuff[0].tx;
  //u8 *buffer =  (u8 *) ...
  //u8 *buffer =  (u8 *)emac_getcurrenttxbuffer();

  for(q = p; q != NULL; q = q->next)
  {
    memcpy((u8_t*)&buffer[l], q->payload, q->len);
    l = l + q->len;
  }
  dm9051_tx_dual(buffer, (uint16_t) l, spipin0);
//if(emac_txpkt_chainmode(l) == ERROR)
//{
//  return ERR_MEM;
//}
  return ERR_OK;
}


/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *
low_level_input(struct netif *netif)
{
  struct pbuf *p, *q;
  u16_t len;
  int l =0;
//  FrameTypeDef frame;
  u8 *buffer = &EthBuff[0].rx;

  len = dm9051_rx(buffer, spipin0); // pin = 0
  if (!len)
	  return NULL;

  /**
   * @brief Calls the function `dm9051_rxlog_monitor_rx_all` to monitor and receive all packets.
   *
   * This function is used to monitor and receive all packets using the `dm9051_rxlog_monitor_rx_all` function.
   * It takes three parameters: `2` (indicating the monitor mode), `buffer` (the buffer to store the received packets), and `len` (the length of the buffer).
   *
   * @param[in] 2       The monitor mode.
   * @param[in] buffer  The buffer to store the received packets.
   * @param[in] len     The length of the buffer.
   */
  // dm9051_rxlog_monitor_rx_all(2, buffer, len);

  /* We allocate a pbuf chain of pbufs from the pool. */
  p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

  if (p != NULL)
  {
    for (q = p; q != NULL; q = q->next)
    {
      memcpy((u8_t*)q->payload, (u8_t*)&buffer[l], q->len);
      l = l + q->len;
    }
  }
  return p;
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
err_t
ethernetif_input(struct netif *netif)
{
  err_t err;
  struct pbuf *p;

  LOCK_TCPIP_CORE();
  /* move received packet into a new pbuf */
  p = low_level_input(netif);
  UNLOCK_TCPIP_CORE();

  /* no packet could be read, silently ignore this */
  if (p == NULL) return ERR_MEM;

  err = netif->input(p, netif);
  if (err != ERR_OK)
  {
    LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
    pbuf_free(p);
    p = NULL;
  }

  return err;
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t
ethernetif_init(struct netif *netif)
{
  struct ethernetif *ethernetif;

  LWIP_ASSERT("netif != NULL", (netif != NULL));

  ethernetif = mem_malloc(sizeof(struct ethernetif));
  if (ethernetif == NULL)
  {
    LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_init: out of memory\n"));
    return ERR_MEM;
  }

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  /*
   * Initialize the snmp variables and counters inside the struct netif.
   * The last argument should be replaced with your link speed, in units
   * of bits per second.
   */
  NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, 100000000);

  netif->state = ethernetif;
  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;
  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
  netif->output = etharp_output;
  netif->linkoutput = low_level_output;

  ethernetif->ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);

  /* initialize the hardware */
  low_level_init(netif);

  return ERR_OK;
}

//typedef struct{
//u32 length;
//u32 buffer;
//emac_dma_desc_type *descriptor;
//}FrameTypeDef;
/*******************************************************************************
* Function Name  : emac_rxpkt_chainmode
* Description    : Receives a packet.
* Input          : None
* Output         : None
* Return         : frame: farme size and location
*******************************************************************************/
//FrameTypeDef emac_rxpkt_chainmode(void)
//{
//	FrameTypeDef frame;;
//	return frame;
//}

