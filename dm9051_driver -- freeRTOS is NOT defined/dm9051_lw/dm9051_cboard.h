#ifndef __DM9051_LW_CBOARD_H__
#define __DM9051_LW_CBOARD_H__

#include "dm9051spiopts.h"
//.#include "../dm9051_lw_mcu_default.h"
//#include "dm9051_lw_conf_types.h"

#ifdef MCU_SPECIFY_CONFIG
#define app_config_board		1
#define driver_config_board		0
#else
#define app_config_board		0
#define driver_config_board		1
#endif

//Location: "dm9051_board_impl.c"
//.#define cint_exint9_5_handler	EXINT9_5_IRQHandler

void DataObj_store(int pin);

void list_extline(uint32_t exint_line);

int dm9051_irq_pincode(uint32_t exint_line);
uint32_t dm9051_irq_exint_line(int pin);

void sprint_hex_dump0(int head_space, int titledn, char *prefix_str,
		    size_t tlen, int rowsize, const void *buf, int seg_start, size_t len, /*int useflg*/ int cast_lf);

#endif //__DM9051_LW_CBOARD_H__
