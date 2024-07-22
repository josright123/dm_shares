/**
  **************************************************************************
  * @file     dm9051_lw_cspi.h
  * @version  v1.0
  * @date     2023-04-28
  * @brief    header file of dm9051 environment config program.
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
#ifndef __DM9051_CSPI_H
#define __DM9051_CSPI_H

//.#include "dm9051_lw_conf_types.h"

/*
 * Include user defined options first. Anything not defined in these files
 * will be set to standard values. Override anything you don't like!
 */

#if freeRTOS && freeRTOS_ENABLE_MUTEX
int cspiSemaphoreDoOwn(int pntlog, char *headstr, SemaphoreHandle_t semaphore_hdlr);
void cspiSemaphoreDoYield(int pntlog, char *headstr, SemaphoreHandle_t semaphore_hdlr);

#define INIT_CSPI_CORE() \
	int rc;
	
#define LOCK_CSPI_CORE(log) \
	if (freeRTOS_ENABLE_MUTEX) \
		rc = cspiSemaphoreDoOwn(log, "empty", NULL);
#define UNLOCK_CSPI_CORE() \
	if (freeRTOS_ENABLE_MUTEX) \
		if (rc) cspiSemaphoreDoYield(0, "empty", NULL);
#else
#define INIT_CSPI_CORE()
#define LOCK_CSPI_CORE(log)
#define UNLOCK_CSPI_CORE()
#endif

#define spi_conf_name()				FIELD_SPIDEV(spidef.spi_name) //spihead().spi_name
#define spi_number()				FIELD_SPIDEV(spidef.spi_num) //spihead().spi_num //= spi_no()
#define spi_crm()					FIELD_SPIDEV(spidef.spi_crm_clk) //spihead().spi_crm_clk

#define spi_iomux()					FIELD_SPIDEV(spidef.iomux)

#define gpio_wire_sck()				FIELD_SPIDEV(wire_sck)
#define gpio_wire_mi()				FIELD_SPIDEV(wire_mi)
#define gpio_wire_mo()				FIELD_SPIDEV(wire_mo)
#define gpio_cs()					FIELD_SPIDEV(wire_cs)

//extern const gpio_mux_t src1_mux6; //= { MUX_DATA(GPIO_PINS_SOURCE1, GPIO_MUX_6)};
//extern const gpio_mux_t src2_mux5; //= { MUX_DATA(GPIO_PINS_SOURCE2, GPIO_MUX_5)};
//extern const gpio_mux_t src4_mux6; //= { MUX_DATA(GPIO_PINS_SOURCE4, GPIO_MUX_6)};
//extern const gpio_mux_t src5_mux5; //= { MUX_DATA(GPIO_PINS_SOURCE5, GPIO_MUX_5)};
//extern const gpio_mux_t src6_mux5; //= { MUX_DATA(GPIO_PINS_SOURCE6, GPIO_MUX_5)};
//extern const gpio_mux_t src7_mux5; //= { MUX_DATA(GPIO_PINS_SOURCE7, GPIO_MUX_5)};

//extern const gpio_mux_t mode_input; //= { INP_DATA()};
//extern const gpio_mux_t mode_output;

#endif //__DM9051_CSPI_H
