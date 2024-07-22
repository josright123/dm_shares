/**
  **************************************************************************
  * @file     dm9051_lw_cint.h
  * @version  v1.0
  * @date     2024-05-29
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
#ifndef __DM9051_CINT_H
#define __DM9051_CINT_H

#define intr_pointer()				FIELD_SPIDEV(intr_cfg)

int intr_gpio_mptr(void); //"dm9051_board_conf_data.h":"dm9051_board_conf.c"

void cint_exint9_5_handler(void);
void cint_disable_mcu_irq(void);
void cint_enable_mcu_irq(void);
//void irq_exint_enable(const struct extscfg_st *pexint_set);

/* [extern] */
#define PTR_EXINTD(field)			FIELD_SPIDEV(intr_cfg)->field

#define intr_gpio_data()			PTR_EXINTD(option)
#define intr_data_scfg()			PTR_EXINTD(extend) //= "FIELD_SPIDEV(intr_cfg)->extend"
#define intr_gpio_info()			PTR_EXINTD(option->gp_info) //NO_USED

/* [intern] */
#define scfg_info()					PTR_EXINTD(scfg_inf)
#define scfg_crm()					PTR_EXINTD(scfg_init.scfg_clk)
#define scfg_port()					PTR_EXINTD(scfg_init.scfg_port_src)
#define scfg_pin()					PTR_EXINTD(scfg_init.scfg_pin_src)

#define exint_enable_info()			PTR_EXINTD(extend->irq_enable_inf)
#define exint_extline()				PTR_EXINTD(extend->extline.extline)
#define exint_crm()					PTR_EXINTD(extend->extline.intr_crm_clk)
#define intr_gpio_ptr()				((const gpio_t *)(&PTR_EXINTD(option->gp)))

#endif //__DM9051_CINT_H
