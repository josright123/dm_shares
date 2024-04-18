/**
 **************************************************************************
 * @file     main.c
 * @version  v2.0.7
 * @date     2022-02-11
 * @brief    main program
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
#include <stdio.h>
#include "dm9051opts.h"
#include "FreeRTOS.h"
#include "task.h"
#include "netconf.h"
#include "ping.h"
#include "at32_dm.h"
#include "httpd.h"
#include "lwip/tcpip.h"
#include "lwiperf.h"
// #include "dm9051_lw_debug.h"
#include "usart3proc/usart3proc_fun.h"
// #include "utility/debug.h"
// #include "utility/esldebug.h"

// #define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG, (fmt, ##__VA_ARGS__))

#include "../freertos_tasks_debug.h" //#include "dm9051_lw_debug.h"
#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_MAIN_ON, /*SEMA_OFF*/ SEMA_ON, "[m]", (fmt, ##__VA_ARGS__))


#define TASK_LIST_BUFFER_SIZE 512

/** @addtogroup UTILITIES_examples
 * @{
 */

/** @addtogroup FreeRTOS_demo
 * @{
 */

extern void udpecho_init(void);
extern void tcpecho_init(void);

TaskHandle_t led2_handler;
TaskHandle_t led3_handler;
TaskHandle_t network_handler;
TaskHandle_t usart3_handler;

/* led2 task */
void led2_task_function(void *pvParameters);
/* led3 task */
void led3_task_function(void *pvParameters);
/* network task */
void network_task_function(void *pvParameters);
/* usart3 task */
void usart3_task_function(void *pvParameters);

void vTaskLwipPerf_function(void *pvParameters);

int spipin0 = 0;
int spipin1 = 1;
/**
 * @brief Prints the task list.
 *
 * This function is used to print the list of tasks in the system.
 * It provides information about each task, such as task name, task state, and task priority.
 *
 * @return void
 */
void printTaskList(void)
{
  char buffer[TASK_LIST_BUFFER_SIZE];

  /* Generate the task list into the buffer. */
  vTaskList(buffer);

  /* Print out the buffer. */
  printf("\r\n%s\r\n", buffer);
}

/**
 * @brief  main function.
 * @param  none
 * @retval none
 */
int main(void)
{
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

  system_clock_config();

  /* init usart1 */
  uart_print_init(115200);
  tasks_dm9051_debug_init();

  /* init led2*/
  at32_led_init(LED2);
  /* init led3*/
  at32_led_init(LED3);
  /* init led4*/
  at32_led_init(LED4);

  /* enter critical */
  taskENTER_CRITICAL();

  /* create led2 task */
  if (xTaskCreate((TaskFunction_t)led2_task_function,
                  (const char *)"LED2_task",
                  (uint16_t)128,
                  (void *)NULL,
                  (UBaseType_t)0,
                  (TaskHandle_t *)&led2_handler) != pdPASS)
  {
    printf("LED2 task could not be created as there was insufficient heap memory remaining.\r\n");
  }
  else
  {
    printf("LED2 task was created successfully.\r\n");
  }

  /* create led3 task */
  if (xTaskCreate((TaskFunction_t)led3_task_function,
                  (const char *)"LED3_task",
                  (uint16_t)128,
                  (void *)NULL,
                  (UBaseType_t)0,
                  (TaskHandle_t *)&led3_handler) != pdPASS)
  {
    printf("LED3 task could not be created as there was insufficient heap memory remaining.\r\n");
  }
  else
  {
    printf("LED3 task was created successfully.\r\n");
  }

  /* create network task */
  if (xTaskCreate((TaskFunction_t)network_task_function,
                  (const char *)"Network_task",
                  (uint16_t)512,
                  (void *)NULL,
                  (UBaseType_t)2,
                  (TaskHandle_t *)&network_handler) != pdPASS)
  {
    printf("Network task could not be created as there was insufficient heap memory remaining.\r\n");
  }
  else
  {
    printf("Network task was created successfully.\r\n");
  }

  /* create usart3_task task */
  if (xTaskCreate((TaskFunction_t)usart3_task_function,
                  (const char *)"USART3_task",
                  (uint16_t)512,
                  (void *)NULL,
                  (UBaseType_t)1,
                  (TaskHandle_t *)&usart3_handler) != pdPASS)
  {
    printf("USART3 task could not be created as there was insufficient heap memory remaining.\r\n");
  }
  else
  {
    printf("USART3 task was created successfully.\r\n");
  }


  /* create vTaskLwipPerf_function task */
  if (xTaskCreate((TaskFunction_t)vTaskLwipPerf_function,
                  (const char *)"vTaskLwipPerf_task",
                  (uint16_t)512,
                  (void *)NULL,
                  (UBaseType_t)2,
                  (TaskHandle_t *)&network_handler) != pdPASS)
  {
    printf("vTaskLwipPerf_function task could not be created as there was insufficient heap memory remaining.\r\n");
  }
  else
  {
    printf("vTaskLwipPerf_function task was created successfully.\r\n");
  }

  /* exit critical */
  taskEXIT_CRITICAL();

  /* start scheduler */
  vTaskStartScheduler();
}

/* led2 task function */
void led2_task_function(void *pvParameters)
{
  while (1)
  {
    // at32_led_toggle(LED2);
    // printf("led2 task function\r\n");
//		size_t xFreeHeapSize = xPortGetFreeHeapSize();
//		printf("Free heap size: %u\r\n", xFreeHeapSize);

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

/* led3 task function */
void led3_task_function(void *pvParameters)
{
  while (1)
  {
    /* for loop delay 500ms */
    // uint32_t us = 50000;
    // const uint32_t us_ticks = 1000;
    // uint32_t i;
    // for(i = 0; i < us * us_ticks; i++)
    // {
    //     __NOP();
    // }
    // printf("led3 task function\r\n");
    at32_led_toggle(LED3);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void network_task_function(void *pvParameters)
{
  ip_addr_t target_ip;
  bannerline_log();
  bannerline_log();
  printf("x2web start: [BOARD_SPI COUNT] %d  /  Operating: [ETHERNET COUNT] %d\r\n", BOARD_SPI_COUNT, ETHERNET_COUNT);

  dm9051_opts_display();

  if (emac_system_init() == ERROR) {
		printf("Error! Whilw(1).\r\n");
		while(1) ;
  }

  tcpip_init(NULL,NULL);
  tcpip_stack_init_2(spipin0);
  tcpip_stack_init_2(spipin1);
  // IP4_ADDR(&target_ip, 192, 168, 249, 134);
  /* 	If ping_init() is not defined
    Check #define LWIP_RAW   1 */
  // ping_init(&target_ip);
  udpecho_init();
  tcpecho_init();
  httpd_init();
	/* Print the task list. */
	printTaskList();
  while (1)
  {
    lwip_pkt_handle();
    vTaskDelay(pdMS_TO_TICKS(10));
//		lwiperf_start_tcp_server_default(NULL, NULL);

  }
}

/* usart3 task function */
void usart3_task_function(void *pvParameters)
{
  usart3_configuration();
  usart3proc_init();
  while (1)
  {
    usart3proc_main();
    // printf("usart3 task function\r\n");
    vTaskDelay(pdMS_TO_TICKS(100));
		usart3proc_time_event(200);				// Check timeout.
  }
}

void vTaskLwipPerf_function(void *pvParameters) {
//	Test code.
  while (1)
  {
    vTaskDelay(pdMS_TO_TICKS(500));
  }
  dm9051_opts_display();
  if (emac_system_init() == ERROR) {
		printf("Error! Whilw(1).\r\n");
		while(1) ;
  }

  tcpip_init(NULL,NULL);
  tcpip_stack_init_2(spipin0);
  udpecho_init();
  tcpecho_init();
	printTaskList();

//		vTaskDelay(pdMS_TO_TICKS(4000));
//    ip_addr_t perf_server_ip;
//    IP_ADDR4(&perf_server_ip, 192, 168, 249, 82);
//    for(;;) {
//				printf("lwiperf_start_tcp_server.\r\n");
//        lwiperf_start_tcp_server(&perf_server_ip, 5201, NULL, NULL);
//        vTaskDelay(pdMS_TO_TICKS(2000));
//    }

//    for(;;) {
//				printf("lwiperf_start_tcp_server.\r\n");
//        lwiperf_start_tcp_server_default(NULL, NULL);
//        vTaskDelay(pdMS_TO_TICKS(2000));
//    }
  while (1)
  {
    lwip_pkt_handle();
//    lwiperf_start_tcp_server_default(NULL, NULL);
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

/**
 * @}
 */

/**
 * @}
 */
