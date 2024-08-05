/**
  **************************************************************************
  * @file     dm_eth_tiny.c
  * @version  v1.0.0
  * @date     2024-06-24
  * @brief    as eth.c
  **************************************************************************
  */
#include "dm_eth_tiny.h"
int flgSemaphore_r = 0;
 
void DM_ETH_IRQHandler(void)
{
	// Inside your ISR
	//BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	// Give the semaphore from the ISR
	//xSemaphoreGiveFromISR(xSemaphore_RX, &xHigherPriorityTaskWoken);
	// If xSemaphoreGiveFromISR unblocked a task of higher priority,
	// request a context switch before exiting the ISR
	//portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

	flgSemaphore_r = 1;
}
