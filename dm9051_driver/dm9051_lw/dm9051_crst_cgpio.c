/**
 **************************************************************************
 * @file     dm9051_lw_crst.c
 * @version  v1.0
 * @date     2024-06-14
 * @brief    dm9051 lwip rst control program
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

//#include "dm9051_lw.h"
#include "dm9051_cboard.h"
#include "../dm9051_lw_mcu_default.h"

#include "dm9051_lw_conf_types.h"

//#include "dm9051_lw_cspi.h"

//#include "dm9051_lw_debug.h"
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
#include "dm9051_lw_crst_cgpio.h"

//#include "dm9051_lw_crst_data.h"
#include "dm9051_lw_conf.h"

//.static void rst_pin_pulse(void) {}

void cpin_poweron_reset(void)
{
	if (rst_pin_exister()) {
		//=rst_pin_pulse();
		gpio_bits_reset(rst_gpio_ptr()->gpport, rst_gpio_ptr()->pin); //rstpin_lo();
		dm_delay_ms(1);
		gpio_bits_set(rst_gpio_ptr()->gpport, rst_gpio_ptr()->pin); //rstpin_hi();
		dm_delay_ms(30);
	}
}

void cpin_gpio_lo(int cpin)
{
	const gp_set_t *gpptr = get_cpin_init_dataptr(cpin);
	if (gpptr)
		gpio_pin_level(gpptr, 0);
}

void cpin_gpio_hi(int cpin)
{
	const gp_set_t *gpptr = get_cpin_init_dataptr(cpin);
	if (gpptr)
		gpio_pin_level(gpptr, 1);
}
