/*
 * Copyright (c) 2017 Simon Goldschmidt
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
 * Author: Simon Goldschmidt
 *
 */
/*
 * Usa as part of sys_arch.c for FreeRTOS.
 */
#include "stdio.h"
#include "cmsis_compiler.h"               /* CMSIS compiler specific defines */
#include "dm9051opts.h"
static int inHandlerMode (void);

#if freeRTOS
xSemaphoreHandle mutex_create_start(void)
{
	xSemaphoreHandle sema;
	sema = xSemaphoreCreateMutex();
	if (!sema)
	  printf("init, failed to create a sema\r\n"); //(ERR_MEM)
	return sema;

	//if (sys_mutex_new(&_lock_dm9051_core) != ERR_OK) {
	//	printf("init, failed to create _lock_dm9051_core\r\n");
	//}
}
#endif /* freeRTOS */

#if freeRTOS
void sys_mutex_lock_start(xSemaphoreHandle *sema_p)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	if(inHandlerMode())
	{
		xSemaphoreTakeFromISR(*sema_p, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
	else
	{
		xSemaphoreTake(*sema_p, portMAX_DELAY);
	}
}
#endif /* freeRTOS */

#if freeRTOS
void sys_mutex_unlock_end(xSemaphoreHandle *sema_p)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	if(inHandlerMode())
	{
		xSemaphoreGiveFromISR(*sema_p, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
	else
	{
		xSemaphoreGive(*sema_p);
	}
}
#endif /* freeRTOS */

/* Determine whether we are in thread mode or handler mode. */
#if freeRTOS
static int inHandlerMode (void)
{
  return __get_IPSR() != 0;
}
#endif /* freeRTOS */
