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
#include "dm9051_lw_debug.h"
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
#include "dm9051_lw_crst_data.h"

void default_rst_add(void)
{
  if (rst_pin_mexist()) {
	/* rst_gpio_ptr() should be correct pin define.
	 */
	printf("gpio_pin_config: RST-gpio %d\r\n", de_pin(rst_gpio_ptr())); //&gpio_wire_mi()//&rst_gpio_ptr()
	gpio_pin_config(rst_gpio_ptr(), GPIO_PULL_UP); //=(rst_gpio_ptr(_pinCode), GPIO_PULL_UP); //,GPIO_MODE_OUTPUT
  }
}

//.static void rst_pin_pulse(void) {}

void default_poweron_reset(void)
{
	if (rst_pin_exister()) {
		//=rst_pin_pulse();
		gpio_bits_reset(rst_gpio_ptr()->gpport, rst_gpio_ptr()->pin); //rstpin_lo();
		dm_delay_ms(1);
		gpio_bits_set(rst_gpio_ptr()->gpport, rst_gpio_ptr()->pin); //rstpin_hi();
		dmf.dly_ms(30);
	}
}

void enum_gpio_add(int cpin_enum)
{
	const gp_set_t *gpptr = get_cpin_init_dataptr(cpin_enum);
	if (gpptr)
		gpio_pin_add(gpptr);
}

flag_status enum_gpio_get_output_data_level(int cpin_enum)
{
	const gp_set_t *gpptr = get_cpin_init_dataptr(cpin_enum);
	if (gpptr)
		return gpio_output_data_bit_read(gpptr->gp.gpport, gpptr->gp.pin);
	return RESET;
}

void enum_gpio_set_output_data_level(int cpin_enum, int level)
{
	const gp_set_t *gpptr = get_cpin_init_dataptr(cpin_enum);
	if (gpptr) {
		if (level == 0)
			gpio_bits_reset(gp_gpio_pt(gpptr)->gpport, gp_gpio_pt(gpptr)->pin); //(&gp_b05) //gen_gpio_ptr(), gen_gpio_ptr()
		else
			gpio_bits_set(gp_gpio_pt(gpptr)->gpport, gp_gpio_pt(gpptr)->pin); //(&gp_b05) //gen_gpio_ptr(), gen_gpio_ptr()
	}
}

//void enum_gpio_set_output_data_lo(int cpin_enum)
//{
//	const gp_set_t *gpptr = get_cpin_init_dataptr(cpin_enum);
//	if (gpptr)
//		gpio_pin_set_level(gpptr, 0);
//}

//void enum_gpio_set_output_data_hi(int cpin_enum)
//{
//	const gp_set_t *gpptr = get_cpin_init_dataptr(cpin_enum);
//	if (gpptr)
//		gpio_pin_set_level(gpptr, 1);
//}

flag_status enum_gpio_get_input_data_level(int cpin_enum)
{
	const gp_set_t *gpptr = get_cpin_init_dataptr(cpin_enum);
	if (gpptr)
		return gpio_input_data_bit_read(gpptr->gp.gpport, gpptr->gp.pin);
	return RESET;
}
