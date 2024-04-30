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
#include "dm9051_lw_conf_types.h"
#include "dm9051_lw_cspi.h"
#include "dm9051_lw_debug.h"

const spi_dev_t *pDevObj[BOARD_SPI_COUNT]; //To be used, in case 'pin_code' can be eliminated mass-quantity.

void DevObj_store(int pin) {
	pDevObj[pin] = PTR_SPIDEV(pin); //'pin_code'
}

void cspi_configuration(void)
{
  /*int i;
  for (i = 0; i < n; i++) { //get_eth_interfaces()
	mstep_set_net_index(i);
	//.printf("Config %s, %s, %s, %02x%02x%02x%02x%02x%02x\r\n",
	//	mstep_conf_info(), mstep_conf_cpu_spi_ethernet(), mstep_conf_cpu_cs_ethernet(),
	//		mac_addresse[mstep_get_net_index()][0],
	//		mac_addresse[mstep_get_net_index()][1],
	//		mac_addresse[mstep_get_net_index()][2],
	//		mac_addresse[mstep_get_net_index()][3],
	//		mac_addresse[mstep_get_net_index()][4],
	//		mac_addresse[mstep_get_net_index()][5]);

	interface_add(i);
  }*/
  
  ETH_COUNT_VOIDFN(DevObj_store);
  ETH_COUNT_VOIDFN(interface_add); //voidfn_dual
  cpin_poweron_reset();
  dmf.dly_ms(30);
}

// -
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
