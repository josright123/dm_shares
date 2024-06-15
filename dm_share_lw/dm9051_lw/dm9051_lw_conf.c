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
#include "dm9051opts.h"
#include "dm9051_lw.h"
//#include "dm9051_lw_conf_types.h"
#include "dm9051_lw_cspi.h"
#include "dm9051_lw_cint.h"

#include "dm9051_lw_debug.h"
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
#include "dm9051_lw_conf_data.h" //1.yicheng 2.later than 'board_printf'

void LIST_EXTLINE(uint32_t exint_line)
{
	int pin;
	if (exint_line)
		printf("Enter exint_line 0x%06x\r\n", exint_line);
	else
		printf("Enter EXINT_LINE_NONE 0x%06x\r\n", exint_line);
		
	for (pin = 0; pin < ETHERNET_COUNT; pin++)
		printf("List%d = 0x%06x\r\n", pin, DataObj_EXINT_extline(pin));
		
	if (exint_line)
		printf("Exit exint_line 0x%06x NOT found!\r\n", exint_line);
	else
		printf("Exit EXINT_LINE_NONE 0x%06x\r\n", exint_line);
}

static uint8_t Is_DataObj_Exist(uint32_t exint_line) {
	int pin;
	for (pin = 0; pin < ETHERNET_COUNT; pin++) {
		if (DataObj_EXINT_extline(pin) == exint_line)
			return 1;
	}
	return 0;
}

static int Get_DataObj_Pin(uint32_t exint_line) {
	int pin;
	for (pin = 0; pin < ETHERNET_COUNT; pin++) {
		if (DataObj_EXINT_extline(pin) == exint_line)
			return pin;
	}
	return -1; //Not the pin
}

int DataObj_EXINT_Pin(uint32_t exint_line)
{
	if (Is_DataObj_Exist(exint_line))
		return Get_DataObj_Pin(exint_line);
	
	LIST_EXTLINE(exint_line);
	return 0;
}
