/**
  **************************************************************************
  * @file     dm9051_lw_conf.h
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
#ifndef __DM9051_ENV_H
#define __DM9051_ENV_H

//tobe "dm9051opts.c"
//
// [This _HELLO_DRIVER_INTERNAL compiler option, is for a diagnostic purpose while the program code is to use this dm9051_lw driver.]
// [Must be 1, for dm9051_lw driver operating.]
// [Set to 0, for the program code to observ the using APIs of dm9051_lw driver, before add the dm9051_lw driver implement files.]
//
#if HELLO_DRIVER_INTERNAL //To support for being called by the program code from outside this dm9051_lw driver.
/*
 * Below access to OPTs data:
 */
//#define IS_GET_INSTEAD(rtype, field)	dm9051opts_get##rtype##field /* definition for extern short-call-name */
//#define IS_SET_INSTEAD(rtype, field)	dm9051opts_set##rtype##field
//#define get_testplaninclude			IS_GET_INSTEAD(enable_t, test_plan_include) //only get is documented export!

//#define u8iomode					OPTS_DATA(uint8_t, iomode)
//#define u8promismode				OPTS_DATA(uint8_t, promismode)
//#define enum_csmode				OPTS_DATA(csmode_t, csmode) //dm9051opts_csmode_tcsmode
//#define enum_bmcrmode				OPTS_DATA(bmcrmode_t, bmcrmode)
//#define isrxtype_test				!OPTS_DATA(enable_t, rxtypemode)
//#define isrxtype_run				OPTS_DATA(enable_t, rxtypemode)
//#define isflowcontrolmode			OPTS_DATA(enable_t, flowcontrolmode)
//#define isrxmode_checksum_offload	OPTS_DATA(enable_t, rxmode_checksum_offload)

#endif //HELLO_DRIVER_INTERNAL

#if 0
	//void mstep_next_net_index(void);
	char *mstep_conf_cpu_spi_ethernet(void);
	char *mstep_spi_conf_name(void);
	//void dm9051_extline_irq_enable(void); //void _dm9051_board_irq_enable(void);
#endif

/* dm9051 delay times procedures */
void dm_delay_us(uint32_t nus);
void dm_delay_ms(uint16_t nms);

#if 0
	char *mstep_conf_info(void);
	char *mstep_conf_cpu_cs_ethernet(void);
	char *mstep_conf_type(void);
	//int mstep_conf_spi_count(void);

	int mstep_dm9051_index(void);

	//static void dm9051_delay_in_core_process(uint16_t nms, char *zhead);
#endif

int display_identity(char *spiname, uint16_t id, uint8_t *ids, uint8_t id_adv, uint16_t idin, char *tail);

#endif //__DM9051_ENV_H
