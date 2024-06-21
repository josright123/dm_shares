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
#include "dm9051_lw.h"

#include "dm9051_lw_cspi.h"
#include "dm9051_lw_crst_data.h"
#include "dm9051_lw_debug.h"
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))

void rst_add(void)
{
  if (rst_pin_mexist()) {
	/* rst_gpio_ptr() should be correct pin define.
	 */
	printf("gpio_pin_config: RST-gpio %d\r\n", de_pin(rst_gpio_ptr())); //&gpio_wire_mi()//&rst_gpio_ptr()
	gpio_pin_config(rst_gpio_ptr(), GPIO_PULL_UP); //=(rst_gpio_ptr(_pinCode), GPIO_PULL_UP); //,GPIO_MODE_OUTPUT
  }
}

void gen_gpio_add(void)
{
  if (gen_gpio_exister()) {
	printf("gpio_pin_config: GEN-gpio %d of %s\r\n", de_pin(gen_gpio_ptr()), gen_gpio_info()); //gen_gpio_data()->gp_info
	gpio_pin_config(gen_gpio_ptr(), GPIO_PULL_UP);
  }
}

static void rst_pin_pulse(void) {
	gpio_bits_reset(rst_gpio_ptr()->gpport, rst_gpio_ptr()->pin); //rstpin_lo();
	dm_delay_ms(1);
	gpio_bits_set(rst_gpio_ptr()->gpport, rst_gpio_ptr()->pin); //rstpin_hi();
}

void cpin_poweron_reset(void)
{
	if (rst_pin_exister())
		rst_pin_pulse(); //.dm9051_if->rstb_pulse()
}

static void gpio_pin_level(int level) {
	if (level == 0)
		gpio_bits_reset(gen_gpio_ptr()->gpport, gen_gpio_ptr()->pin); //rstpin_lo();
	else
		gpio_bits_set(gen_gpio_ptr()->gpport, gen_gpio_ptr()->pin); //rstpin_hi();
}

void cpin_gpio_lo(void)
{
	if (gen_gpio_exister())
		gpio_pin_level(0); //.dm9051_if->rstb_pulse()
}

void cpin_gpio_hi(void)
{
	if (gen_gpio_exister())
		gpio_pin_level(1); //.dm9051_if->rstb_pulse()
}
