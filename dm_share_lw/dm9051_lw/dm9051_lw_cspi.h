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

#include "dm9051_lw_conf_types.h"
#define spi_number()				FIELD_SPIDEV(spidef.spi_num) //spihead().spi_num //= spi_no()
#define spi_crm()					FIELD_SPIDEV(spidef.spi_crm_clk) //spihead().spi_crm_clk
#define gpio_wire_sck()				FIELD_SPIDEV(wire_sck)
#define gpio_wire_mi()				FIELD_SPIDEV(wire_mi)
#define gpio_wire_mo()				FIELD_SPIDEV(wire_mo)
#define gpio_cs()					FIELD_SPIDEV(wire_cs)

#define spi_iomux()					FIELD_SPIDEV(spidef.iomux)

void gpio_pin_config(const gpio_t *gpio, gpio_pull_type gppull);
void spi_add(void);

/* used in struct data declaration */
#define SPI_PINSTD(spiname,spinum,crm_clk,iom)		{spiname,spinum,crm_clk, iom}
#define GPIO_PINNORM(gpport,pin,crm_clk)			{gpport,pin,crm_clk, GPIO_MODE_MUX,		GPIO_PINSRC_NULL, GPIO_MUX_NULL}
#define GPIO_PINMUX(gpport,pin,crm_clk,pinsrc,mux)	{gpport,pin,crm_clk, GPIO_MODE_MUX,		pinsrc,mux}
#define GPIO_PINOUT(gpport,pin,crm_clk)				{gpport,pin,crm_clk, GPIO_MODE_OUTPUT,	GPIO_PINSRC_NULL, GPIO_MUX_NULL}

#endif //__DM9051_CSPI_H
