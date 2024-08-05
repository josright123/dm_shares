#ifndef __DM_ETH_H__
#define __DM_ETH_H__

//#include "lwip/netif.h" //#include "lwip/err.h"

//extern SemaphoreHandle_t xSemaphore_RX;
////extern int flgSemaphore_r;

//void ethernetif_link_log_reset(void);

//uint32_t DM_Eth_Pid_and_bmsr(uint8_t link_up);
//void DM_ETH_Init(struct netif *netif, const uint8_t *adr);
//void DM_Eth_Initialize(void);
////.void LWIP_Initalize(void);
////static void UIP_Initalize(void);
////.void DM_ETH_RXHandler(void* param);
//struct pbuf *DM_ETH_Input(struct netif *netif);
//err_t DM_ETH_Output(struct netif *netif, struct pbuf *p);

//void DM_ETH_IRQHandler(void);

//void DM_ETH_IRQEnable(void);
//uint16_t DM_ETH_IRQDisable(void);

void DM_ETH_IRQHandler(void);

#endif //__DM_ETH_H__
