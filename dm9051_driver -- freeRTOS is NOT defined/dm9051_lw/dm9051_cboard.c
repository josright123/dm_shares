/**
 **************************************************************************
 * @file     dm9051_cboard.c
 * @version  v1.0
 * @date     2024-06-27
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
#include "dm9051_cboard.h"
#include "../dm9051_lw_mcu_default.h"
#include "dm9051_lw_conf_types.h"
#include "dm9051_lw_cspi.h"
#include "dm9051_lw_cint.h"
#include "dm9051_lw_crst_cgpio.h"

#include "dm9051_lw_log.h"
#include "dm9051_lw_debug.h"
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
#include "../freertos_tasks_debug.h"
#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_NETCONF_ON, /*SEMA_OFF*/ SEMA_ON, "[C]", (fmt, ##__VA_ARGS__))

void dm9051_boards_initialize(void)
{
  DM9051_BOARD_CFG((app_config_board), "application_config_board", cboard_conf_configuration()); //app config board
}

void cboard_conf_configuration(void) //(uint8_t specified)
{
	dm9051_board_counts_display("freertos_f437_dm9051");

	//if (specified) {
	  ETH_COUNT_VOIDFN(DataObj_store);
	  ETH_COUNT_VOIDFN(interface_all_add);
	//}
}

void dm9051_board_counts_display(const char *project_name) {
	bannerline_log();
	bannerline_log();
	printf("%s start: [BOARD_SPI COUNT] %d  /  Operating: [ETHERNET COUNT] %d\r\n", project_name, BOARD_SPI_COUNT, ETHERNET_COUNT);
	bannerline_log();
}

//[Add GPIO]
/*********************************
  * @brief  gpio pin configuration
  * @brief  spi configuration
  * @brief  exint configuration
 *********************************/
//static 
void gpio_pin_config(const gpio_t *gpio, gpio_pull_type gppull) //, gpio_mode_type gpmode
{
  gpio_init_type gpio_init_struct;
  crm_periph_clock_enable(gpio->gp_crmclk, TRUE); /* enable the gpioa clock */

  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_out_type  		= GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_drive_strength	= GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_mode			= gpio->pmux->gpio_mode; //gmode
											//gpio->gpio_mode; //gpmode; //GPIO_MODE_INPUT;

  gpio_init_struct.gpio_pull			= gppull; //exint_cfg.gpio_pull; //GPIO_PULL_DOWN; GPIO_PULL_UP; //GPIO_PULL_NONE;
  gpio_init_struct.gpio_pins			= gpio->pin;
  gpio_init(gpio->gpport, &gpio_init_struct);
  
 #ifdef AT32F437xx
 
  //temp
  if (!gpio->pmux) {
	printf("------ - - - NOT allow a NULL pmux field - - - ------\r\n");
	return;
  }

  if (gpio->pmux->gpio_mode == GPIO_MODE_MUX) {
		gpio_pin_mux_config(gpio->gpport, gpio->pmux->pinsrc, gpio->pmux->muxsel);
		return;
  }
 #endif
}

//[spi configuration]
/**
  * @brief  spi configuration.
  * @param  spi_inf_t* =
  *         struct {
  *         	spi_type *num;        			//= SPIPORT;
  *         	crm_periph_clock_type spi_crm_clk;	//= SPI_CRM_CLK;
  *         } spi_inf_t;
  * @retval None
  */
//static 
void spi_config(void)
{
  spi_init_type spi_init_struct;
  crm_periph_clock_enable(spi_crm(), TRUE); //crm_spi_clk(_pinCode), CRM_SPI3_PERIPH_CLOCK/CRM_SPI2_PERIPH_CLOCK

  spi_default_para_init(&spi_init_struct);
  spi_init_struct.transmission_mode = SPI_TRANSMIT_FULL_DUPLEX;
  spi_init_struct.master_slave_mode = SPI_MODE_MASTER;
  spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_8;
  //spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_LSB;
  spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_MSB;
  spi_init_struct.frame_bit_num = SPI_FRAME_8BIT;
  spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_LOW;
  //spi_init_struct.clock_phase = SPI_CLOCK_PHASE_2EDGE;
  spi_init_struct.clock_phase = SPI_CLOCK_PHASE_1EDGE;
  spi_init_struct.cs_mode_selection = SPI_CS_SOFTWARE_MODE;
  spi_init(spi_number(), &spi_init_struct); //SPI2, _spi_num(_pinCode)
  spi_enable(spi_number(), TRUE); //SPI2, _spi_num(_pinCode)
}

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
//void exint_mconfig(exint_polarity_config_type polarity)
//{
//}

/**
  * @brief  gpio configuration.
  * @brief  spi configuration.
  * @brief  exint configuration.
  */
//static
void spi_add(void) //=== pins_config(); //total_eth_count++;
{
//.#ifndef AT32F437xx
  //Setting of Non-f437
  if (spi_number() == SPI1) {
	  if  (spi_iomux() & IO_CRM_CLOCK) {

		#if !defined(AT32F437xx) || defined(AT32F413xx) || defined(AT32F415xx) || \
			defined(AT32F403Axx) || defined(AT32F403xx) || defined(AT32F407xx)
	    crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE); //Non-f437,iomux-config
	    gpio_pin_remap_config(SWJTAG_GMUX_010, TRUE); //Non-f437,iomux-config
		#endif //.
	  }
	  if  (spi_iomux() & IO_MUX_PINREMAP) {

		#if !defined(AT32F437xx) || defined(AT32F413xx) || defined(AT32F415xx) || \
			defined(AT32F403Axx) || defined(AT32F403xx) || defined(AT32F407xx)
		  gpio_pin_remap_config(SPI1_MUX_01, TRUE); //Non-f437,remap
		#endif //.
	  }
  }
//.#endif
  bannerline_log();
  printf("gpio_pin_config: SCK. GPIO_PINS_%d\r\n", de_pin(&gpio_wire_sck()));
  printf("gpio_pin_config: MI. GPIO_PINS_%d\r\n", de_pin(&gpio_wire_mi()));
  printf("gpio_pin_config: MO. GPIO_PINS_%d\r\n", de_pin(&gpio_wire_mo()));
  printf("gpio_pin_config: CS. GPIO_PINS_%d\r\n", de_pin(&gpio_cs()));
  
  gpio_pin_config(&gpio_wire_sck(), GPIO_PULL_NONE); //,GPIO_MODE_MUX
  gpio_pin_config(&gpio_wire_mi(), GPIO_PULL_NONE); //,GPIO_MODE_MUX
  gpio_pin_config(&gpio_wire_mo(), GPIO_PULL_NONE); //,GPIO_MODE_MUX //GPIO_PULL_UP; //test ffff

  spi_config(); //(spi_port_ptr(_pinCode));
  gpio_pin_config(&gpio_cs(), GPIO_PULL_NONE); //,GPIO_MODE_OUTPUT
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

  if (intr_pointer())
  {
    printf("................................ dm9051 (INT)\r\n");
    if (intr_gpio_data())
    {
      printf(".gpio_pin_config: INTR. GPIO_PINS_%d\r\n", de_pin(intr_gpio_ptr())); //" %d", de_pin(&intr_gpio_ptr())
      gpio_pin_config(intr_gpio_ptr(), confi.gppull);
    }
    else
      printf(".gpio_pin_config: INTR-gpio (omitted)\r\n");
    //printf("................................ dm9051 gpio_pin_config (INT)\r\n");
  }
  else
    printf("................................ dm9051 (POLLING)\r\n");

  if (intr_pointer())
  {
    const struct extscfg_st *pexint_set = (const struct extscfg_st *)intr_data_scfg(); //exint_scfg_ptr();
    if (pexint_set)
    {
      printf(".exint_config: %s\r\n", scfg_info());
	  identify_irq_stat(ISTAT_IRQ_CFG);
      exint_config(pexint_set, confi.polarity);
    }
	else 
      printf(".exint_config: (INT) NO pexint_set?\r\n");
    //printf(".exint_config (INT)\r\n");
  }
  else
    printf(".exint_config (POLLING)\r\n");
}

void cpin_rst_add(void)
{
  if (rst_pin_mexist()) {
	/* rst_gpio_ptr() should be correct pin define.
	 */
	printf("gpio_pin_config: RST. GPIO_PINS_%d\r\n", de_pin(rst_gpio_ptr())); //&gpio_wire_mi()//&rst_gpio_ptr()
	gpio_pin_config(rst_gpio_ptr(), GPIO_PULL_UP); //=(rst_gpio_ptr(_pinCode), GPIO_PULL_UP); //,GPIO_MODE_OUTPUT
  }
}

static void gpio_pin_add(const gp_set_t *gpptr)
{
//#undef printf
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
  //if (gen_gpio_exister()) {
	printf("gpio_pin_config: USER_GEN-gpio. GPIO_PINS_%d of %s\r\n", de_pin(gp_gpio_pt(gpptr)), gp_info(gpptr)); //gen_gpio_ptr(), //gen_gpio_info(), gen_gpio_data()->gp_info
	gpio_pin_config(gp_gpio_pt(gpptr), GPIO_PULL_UP); //gen_gpio_ptr()
  //}
//#undef printf
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

void cpin_gpio_add(int cpin)
{
	const gp_set_t *gpptr = get_cpin_init_dataptr(cpin);
	if (gpptr)
		gpio_pin_add(gpptr);
}

void interface_all_add(int pin)
{
	DM_UNUSED_ARG(pin);

	spi_add();
	intr_add();
	cpin_rst_add();

//	cpin_gpio_add(CPIN_PB05);
}
