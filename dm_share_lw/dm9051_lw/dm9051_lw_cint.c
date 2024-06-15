/**
 **************************************************************************
 * @file     dm9051_lw_cint.c
 * @version  v1.0
 * @date     2024-05-29
 * @brief    dm9051 lwip intr control program
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
// #include "dm9051_lw_conf_types.h"
#include "dm9051_lw_cspi.h"
#include "dm9051_lw_cint.h"
#include "dm9051_lw_cint_data.h" //1.yicheng
#include "dm9051_lw_debug.h"

//[Add int]
/**
 * @brief  exint pin configuration.
 * @param  struct exint_st* =
 * 	   struct exint_st {
 * 		struct {
 * 			crm_periph_clock_type crmclk; //CRM_GPIOC_PERIPH_CLOCK,
 * 			uint32_t extline; //= LINE
 * 			IRQn_Type irqn; //= EXINTn_m
 * 		} extline;
 * 		struct {
 * 			crm_periph_clock_type scfg_clk; //CRM_SCFG_PERIPH_CLOCK
 * 			scfg_port_source_type scfg_port_src; //SCFG_PORT_SOURCE_X
 * 			scfg_pins_source_type scfg_pin_src; //SCFG_PINS_SOURCEX
 * 		} scfg_init;
 * 	   }
 *        exint_polarity_config_type
 *        nvic_priority_group_type
 * @e.g.  exint line7 config. configure pc7 in interrupt pin
 * @retval None
 */
static void exint_config(const struct extscfg_st *pexint_set, exint_polarity_config_type polarity)
{
  exint_init_type exint_init_struct;

  printf(": %s :                 exint_config/ %s\r\n", "config", scfg_info());

  crm_periph_clock_enable(scfg_crm(), TRUE);  // CRM_SCFG_PERIPH_CLOCK
  crm_periph_clock_enable(exint_crm(), TRUE); // CRM_GPIOC_PERIPH_CLOCK

#ifndef AT32F437xx
  gpio_exint_line_config(scfg_port(), scfg_pin()); // SCFG_PORT_SOURCE_GPIOA, SCFG_PINS_SOURCE0
#else
  scfg_exint_line_config(scfg_port(), scfg_pin()); // SCFG_PORT_SOURCE_GPIOC, SCFG_PINS_SOURCE7
  //  printf(": %s, of AT32F437\r\n", scfg_info());
  //  bannerline_log();
#endif

  exint_default_para_init(&exint_init_struct);
  exint_init_struct.line_enable = TRUE;

  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_select = pexint_set->extline.extline; // line_no;
  exint_init_struct.line_polarity = polarity;                  // EXINT_TRIGGER_RISING_EDGE/ EXINT_TRIGGER_FALLING_EDGE
  exint_init(&exint_init_struct);
}

//-caller
void exint_mconfig(exint_polarity_config_type polarity)
{
  if (intr_pointer())
  {
    const struct extscfg_st *pexint_set = (const struct extscfg_st *)exint_scfg_ptr();
    printf(".exint_mconfig (INT)\r\n");
    if (pexint_set)
    {
      exint_config(pexint_set, polarity);
    }
  }
  else
    printf(".exint_mconfig (POLLING)\r\n");
}

void intr_add(void)
{
  //  config_exint(GPIO_PULL_UP, EXINT_TRIGGER_FALLING_EDGE);
  //  gpio_pull_type gppull = GPIO_PULL_UP;
  //  exint_polarity_config_type polarity = EXINT_TRIGGER_FALLING_EDGE;
  struct
  {
    gpio_pull_type gppull;
    exint_polarity_config_type polarity;
  } confi = {
      GPIO_PULL_UP,
      EXINT_TRIGGER_FALLING_EDGE,
  };

  if (intr_gpio_mptr())
  {
    //printf("................................ dm9051 gpio_pin_config (INT)\r\n");

    if (intr_gpio_data())
    {
      printf("gpio_pin_config: INTR-gpio %d\r\n", de_pin(intr_gpio_ptr())); //" %d", de_pin(&intr_gpio_ptr())
      gpio_pin_config(intr_gpio_ptr(), confi.gppull);
    }
    else
      printf("gpio_pin_config: INTR-gpio (omitted)\r\n");
  }
  else
    printf("................................ dm9051 (POLLING)\r\n");

  exint_mconfig(confi.polarity);
}

int is_dm9051_board_irq(void)
{
  const struct extscfg_st *pexint_set = NULL;
  if (intr_pointer())
  {
    printf(".is_dm9051_irq (INT)\r\n");
    pexint_set = (const struct extscfg_st *)exint_scfg_ptr();
  }
  else
    printf(".is_dm9051_irq (POLLING)\r\n");
  return pexint_set ? 1 : 0;
}

//[Enable int]
static void exint_enable(const struct extscfg_st *pexint_set)
{

  //	printf(": %s :                 exint-enable/ %s\r\n", "config", exint_enable_info()); //pexint_set
  bannerline_log();
  printf("irq_priority[%d]: priority = 0x%x !\r\n", mstep_get_net_index(), pexint_set->extline.priority); // or "NVIC_PRIORITY_GROUP_0"
  printf("irq_enable[%d]: %s\r\n", mstep_get_net_index(), exint_enable_info());                           // pexint_set
  bannerline_log();

  // NVIC_PRIORITY_GROUP_0/NVIC_PRIORITY_GROUP_4, //nvic_priority_group_type priority
  nvic_priority_group_config(pexint_set->extline.priority);
  nvic_irq_enable(pexint_set->extline.irqn, 1, 0); // nvic_irq_enable(EXINT9_5_IRQn, 1, 0); //i.e.= //_misc
}

void dm9051_board_irq_enable(void)
{
  printf("\r\n");
  if (intr_pointer())
  {
    const struct extscfg_st *pexint_set = (const struct extscfg_st *)exint_scfg_ptr();
    printf(".dm9051_board_irq_enable (INT)\r\n");
    if (pexint_set)
    {
      exint_enable(pexint_set);
      // #if ETHERNET_INTERRUPT_MODE
      //		DEBUG_refresh_isr_check();
      // #endif
    }
  }
  else
    printf(".dm9051_board_irq_enable (POLLING)\r\n");
}

/**
 * @brief  exint line0 config. configure pa0 in interrupt mode
 * @param  None
 * @retval None
 */
// void exint_line0_config(void)
//{
//   exint_init_type exint_init_struct;

//  crm_periph_clock_enable(CRM_SCFG_PERIPH_CLOCK, TRUE);
//  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

//  scfg_exint_line_config(SCFG_PORT_SOURCE_GPIOA, SCFG_PINS_SOURCE0);

//  exint_default_para_init(&exint_init_struct);
//  exint_init_struct.line_enable = TRUE;
//
//  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
//  exint_init_struct.line_select = EXINT_LINE_0;
//  exint_init_struct.line_polarity = EXINT_TRIGGER_FALLING_EDGE; //EXINT_TRIGGER_RISING_EDGE;
//  exint_init(&exint_init_struct);

//  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
//  nvic_irq_enable(EXINT0_IRQn, 1, 0);
//}
// void exint_line4_config(void)
//{
//  exint_init_type exint_init_struct;
//	nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);

//  crm_periph_clock_enable(CRM_SCFG_PERIPH_CLOCK, TRUE);

//  exint_default_para_init(&exint_init_struct);
//  exint_init_struct.line_enable = TRUE;
//  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
//  exint_init_struct.line_select = EXINT_LINE_4;
//  exint_init_struct.line_polarity = EXINT_TRIGGER_RISING_EDGE;
//  exint_init(&exint_init_struct);
//  exint_flag_clear(EXINT_LINE_4);
//  nvic_irq_enable(EXINT4_IRQn, 1, 0);
//}
