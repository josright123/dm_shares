/**
  **************************************************************************
  * @file     dm9051_lw_conf.c
  * @file     dm9051_at32_conf.c
  * @file     dm9051_env.c ,dm9051_config.c ,at32_emac.c
  * @version  v1.0
  * @date     2023-04-28, 2023-10-17, 2023-10-24
  * @brief    dm9051 config program
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
#include "stdio.h"
#include "dm9051opts.h"
#include "dm9051_lw.h"
#include "dm9051_cboard.h"
#include "dm9051_lw_cspi.h"
#include "dm9051_lw_cint.h"
#include "dm9051_lw_data.h" //1.yicheng 2.later than 'board_printf'

//#include "dm9051_lw_debug.h"
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
#include "../freertos_tasks_debug.h"
#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_NETCONF_ON, /*SEMA_OFF*/ SEMA_ON, "[F]", (fmt, ##__VA_ARGS__))

/*
 * To eliminate this implement file to totally simple as, no optional case.
 */

//#if freeRTOS
//void FreeDelayus(uint32_t nus) {
//	uint32_t ustime = xTaskGetTickCount();
//	while (xTaskGetTickCount() < (ustime + nus)) ;
//}
//void FreeDelayms(uint32_t nms) {
//	FreeDelayus(nms*100);
//}
//#endif

#if freeRTOS
// 對於微秒級的延遲，您可能需要使用忙等待迴圈或硬體支援的計時器。
// void delay_us(uint32_t us)
// {
//     // 假設系統時鐘為 100MHz，請根據您的硬體調整此值
//     const uint32_t us_ticks = 100;
//     uint32_t i;
//     for(i = 0; i < us * us_ticks; i++)
//     {
//         __NOP();
//     }
// }
static void uvTaskDelay( const TickType_t xTicksToDelay ) {
	vTaskDelay(pdMS_TO_TICKS((xTicksToDelay + 999)/ 1000));
}
#else
#include "../dm9051_lw_mcu_default.h"
#endif

	const dm_dly_t dmf = {
	#if freeRTOS
		uvTaskDelay, //here assign, define system's delay us function
		vTaskDelay, //here assign, define system's delay ms function
	#else
		delay_us, //here assign, define system's delay us function
		delay_ms, //here assign, define system's delay ms function
	#endif
	};

	void dm_delay_us(uint32_t nus)
	{
		#define	board_printf(format, args...)
		board_printf("test %d ,because rxb %02x (is %d times)\r\n", rstccc, rxbyteee, timesss);
		dmf.dly_us(nus); //delay_us(nus);
	}

	void dm_delay_ms(uint16_t nms)
	{
		dmf.dly_ms(nms); //delay_ms(nms);
	}

int display_identity(char *spiname, uint16_t id, uint8_t *ids, uint8_t id_adv, uint16_t idin, char *tail)
{
//	#undef printf
//	#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	
	DM_UNUSED_ARG(id);
	DM_UNUSED_ARG(id_adv);
	if (ids) {
	}
	printf("%s[%d] %s ::: Chip ID %04x %s\r\n",
		": Read device", //display_identity_bannerline_default=
		0, spiname,
		idin,
		tail
		);
	return 0;
//	#undef printf
//	#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}
