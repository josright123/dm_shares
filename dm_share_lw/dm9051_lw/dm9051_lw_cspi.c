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
#include "dm9051_lw_cspi_data.h"

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
static void spi_config(void)
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
  crm_periph_clock_enable(gpio->gpio_crm_clk, TRUE); /* enable the gpioa clock */

  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_out_type  		= GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_drive_strength	= GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_mode			= gpio->gpio_mode; //gpmode; //GPIO_MODE_INPUT;

  gpio_init_struct.gpio_pull			= gppull; //exint_cfg.gpio_pull; //GPIO_PULL_DOWN; GPIO_PULL_UP; //GPIO_PULL_NONE;
  gpio_init_struct.gpio_pins			= gpio->pin;
  gpio_init(gpio->gpport, &gpio_init_struct);

 #ifdef AT32F437xx
  if ((gpio->gpio_mode == GPIO_MODE_MUX) && (gpio->muxsel != GPIO_MUX_NULL))
	gpio_pin_mux_config(gpio->gpport, gpio->pinsrc, gpio->muxsel);
  #if 0
  else {
    // only for interr-gpio
	/*if (intr_gpio_exister())*/
		// only for interr-gpio
		printf("f437 : %s, no gpio_pin_mux_config()\r\n", intr_gpio_info());
  }
  #endif
 #endif
}

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

		#ifndef AT32F437xx //.
	    crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE); //Non-f437,iomux-config
	    gpio_pin_remap_config(SWJTAG_GMUX_010, TRUE); //Non-f437,iomux-config
		#endif //.
	  }
	  if  (spi_iomux() & IO_MUX_PINREMAP) {

		#ifndef AT32F437xx //.
		  gpio_pin_remap_config(SPI1_MUX_01, TRUE); //Non-f437,remap
		#endif //.
	  }
  }
//.#endif
  bannerline_log();
  printf("gpio_pin_config: SCK-gpio\r\n");
  gpio_pin_config(&gpio_wire_sck(), GPIO_PULL_NONE); //,GPIO_MODE_MUX
  printf("gpio_pin_config: MI-gpio\r\n");
  gpio_pin_config(&gpio_wire_mi(), GPIO_PULL_NONE); //,GPIO_MODE_MUX
  printf("gpio_pin_config: MO-gpio\r\n");
  gpio_pin_config(&gpio_wire_mo(), GPIO_PULL_NONE); //,GPIO_MODE_MUX //GPIO_PULL_UP; //test ffff

  spi_config(); //(spi_port_ptr(_pinCode));
  printf("gpio_pin_config: CS-gpio\r\n");
  gpio_pin_config(&gpio_cs(), GPIO_PULL_NONE); //,GPIO_MODE_OUTPUT
}

/*********************************
 * dm9051 spi interface accessing
 *********************************/
 
#define dm9051if_cs_lo() spi_cs_lo()
#define dm9051if_cs_hi() spi_cs_hi()
#define dm9051_spi_command_write(rd) spi_exc_data(rd)
#define dm9051_spi_dummy_read() spi_exc_data(0)

static __inline void spi_cs_lo(void) {
	gpio_bits_reset(gpio_cs().gpport, gpio_cs().pin); //cs.gpport->clr = cs.pin;
}
static __inline void spi_cs_hi(void) {
	gpio_bits_set(gpio_cs().gpport, gpio_cs().pin); //cs.gpport->scr = cs.pin;
}

static __inline uint8_t spi_exc_data(uint8_t byte) { 
    while(spi_i2s_flag_get(spi_number(), SPI_I2S_TDBE_FLAG) == RESET);	//while(spi_i2s_flag_get(SPI2, SPI_I2S_TDBE_FLAG) == RESET);
    spi_i2s_data_transmit(spi_number(), byte);							//spi_i2s_data_transmit(SPI2, byte); //spi2 tx
    while(spi_i2s_flag_get(spi_number(), SPI_I2S_RDBF_FLAG) == RESET);	//while(spi_i2s_flag_get(SPI2, SPI_I2S_RDBF_FLAG) == RESET);
    return (uint8_t) spi_i2s_data_receive(spi_number());				//return (uint8_t) spi_i2s_data_receive(SPI2); //spi2 rx (rx_pad)
}

void cspi_read_regs(uint8_t reg, uint8_t *buf, uint16_t len, csmode_t csmode)
{
	int i;
	int par_regs = (reg == DM9051_PAR);

	if (csmode == CS_LONG) {
#if freeRTOS_ENABLE_MUTEX && (JOS_MUTEX_LO_INIT || JOS_MUTEX_LINK || JOS_MUTEX_RX || JOS_MUTEX_TX)
	INIT_CSPI_CORE()
	LOCK_CSPI_CORE(0)
#endif
	  dm9051if_cs_lo();
	  for(i=0; i < len; i++, reg++) {
		dm9051_spi_command_write(reg | OPC_REG_R);
		buf[i] = dm9051_spi_dummy_read();
		if (par_regs)
		  printf("long read reg %02x = %02x\r\n", reg, buf[i]);
	  }
	  dm9051if_cs_hi();
#if freeRTOS_ENABLE_MUTEX && (JOS_MUTEX_LO_INIT || JOS_MUTEX_LINK || JOS_MUTEX_RX || JOS_MUTEX_TX)
	UNLOCK_CSPI_CORE()
#endif
	}
	else { //CS_EACH
	  for(i=0; i < len; i++, reg++) {
		//printf("cspi_read_reg(reg) ..\r\n");
		buf[i] = cspi_read_reg(reg);
		if (par_regs)
		  ; //printf("each read reg %02x = %02x\r\n", reg, buf[i]);
	  }
	}
}

uint8_t cspi_read_reg(uint8_t reg) //static (todo)
{
	uint8_t val;
#if freeRTOS_ENABLE_MUTEX && (JOS_MUTEX_LO_INIT || JOS_MUTEX_LINK || JOS_MUTEX_RX || JOS_MUTEX_TX)
	INIT_CSPI_CORE()
	LOCK_CSPI_CORE(0)
#endif
	dm9051if_cs_lo();
	dm9051_spi_command_write(reg | OPC_REG_R);
	val = dm9051_spi_dummy_read();
	dm9051if_cs_hi();
#if freeRTOS_ENABLE_MUTEX && (JOS_MUTEX_LO_INIT || JOS_MUTEX_LINK || JOS_MUTEX_RX || JOS_MUTEX_TX)
	UNLOCK_CSPI_CORE()
#endif
	return val;
}
void cspi_write_reg(uint8_t reg, uint8_t val)
{
#if freeRTOS_ENABLE_MUTEX && (JOS_MUTEX_LO_INIT || JOS_MUTEX_LINK || JOS_MUTEX_RX || JOS_MUTEX_TX)
	INIT_CSPI_CORE()
	LOCK_CSPI_CORE(0)
#endif
	dm9051if_cs_lo();
	dm9051_spi_command_write(reg | OPC_REG_W);
	dm9051_spi_command_write(val);
	dm9051if_cs_hi();
#if freeRTOS_ENABLE_MUTEX && (JOS_MUTEX_LO_INIT || JOS_MUTEX_LINK || JOS_MUTEX_RX || JOS_MUTEX_TX)
	UNLOCK_CSPI_CORE()
#endif
}
uint8_t cspi_read_mem2x(void)
{
	uint8_t rxb;
#if freeRTOS_ENABLE_MUTEX && (JOS_MUTEX_LO_INIT || JOS_MUTEX_LINK || JOS_MUTEX_RX || JOS_MUTEX_TX)
	INIT_CSPI_CORE()
	LOCK_CSPI_CORE(1)
#endif
	dm9051if_cs_lo();
	dm9051_spi_command_write(DM9051_MRCMDX | OPC_REG_R);
	rxb = dm9051_spi_dummy_read();
	rxb = dm9051_spi_dummy_read();
	dm9051if_cs_hi();
#if freeRTOS_ENABLE_MUTEX && (JOS_MUTEX_LO_INIT || JOS_MUTEX_LINK || JOS_MUTEX_RX || JOS_MUTEX_TX)
	UNLOCK_CSPI_CORE()
#endif
	return rxb;
}
void cspi_read_mem(u8 *buf, u16 len)
{
	int i;
#if freeRTOS_ENABLE_MUTEX && (JOS_MUTEX_LO_INIT || JOS_MUTEX_LINK || JOS_MUTEX_RX || JOS_MUTEX_TX)
	INIT_CSPI_CORE()
	LOCK_CSPI_CORE(1)
#endif
	dm9051if_cs_lo();
	dm9051_spi_command_write(DM9051_MRCMD | OPC_REG_R);
	if (!OPT_CONFIRM(onlybytemode) && (OPT_U8(iomode) == MBNDRY_WORD)) //u8iomode() == MBNDRY_WORD, dm9051opts_iomode(), MBNDRY_DEFAULT == MBNDRY_WORD
	if (len & 1)
		len++;
	for(i=0; i<len; i++)
		buf[i] = dm9051_spi_dummy_read();
	dm9051if_cs_hi();
#if freeRTOS_ENABLE_MUTEX && (JOS_MUTEX_LO_INIT || JOS_MUTEX_LINK || JOS_MUTEX_RX || JOS_MUTEX_TX)
	UNLOCK_CSPI_CORE()
#endif
}
void cspi_write_mem(u8 *buf, u16 len)
{
	int i;
#if freeRTOS_ENABLE_MUTEX && (JOS_MUTEX_LO_INIT || JOS_MUTEX_LINK || JOS_MUTEX_RX || JOS_MUTEX_TX)
	INIT_CSPI_CORE()
	LOCK_CSPI_CORE(0)
#endif
	dm9051if_cs_lo();
	dm9051_spi_command_write(DM9051_MWCMD | OPC_REG_W);
	if (!OPT_CONFIRM(onlybytemode) && (OPT_U8(iomode) == MBNDRY_WORD)) //u8iomode() == MBNDRY_WORD, dm9051opts_iomode(), MBNDRY_DEFAULT == MBNDRY_WORD
	if (len & 1)
		len++;
	for(i=0; i<len; i++)
		dm9051_spi_command_write(buf[i]);
	dm9051if_cs_hi();
#if freeRTOS_ENABLE_MUTEX && (JOS_MUTEX_LO_INIT || JOS_MUTEX_LINK || JOS_MUTEX_RX || JOS_MUTEX_TX)
	UNLOCK_CSPI_CORE()
#endif
}
