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
#include "dm9051opts.h"
#if defined (AT32F403Axx)
#include <stdint.h>
#include "at32f403a_407_conf.h"
#endif

//tobe "dm9051opts.c"
//
// [This _HELLO_DRIVER_INTERNAL compiler option, is for a diagnostic purpose while the program code is to use this dm9051_lw driver.]
// [Must be 1, for dm9051_lw driver operating.]
// [Set to 0, for the program code to observ the using APIs of dm9051_lw driver, before add the dm9051_lw driver implement files.]
//

//init
#if DM9051OPTS_API
void dm9051_boards_initialize(int n);
uint8_t *identify_eth_mac(uint8_t *macadr, uint8_t pin);
uint8_t *identify_tcpip_ip(uint8_t *ip4adr, uint8_t pin);
uint8_t *identify_tcpip_gw(uint8_t *ip4adr, uint8_t pin);
uint8_t *identify_tcpip_mask(uint8_t *ip4adr, uint8_t pin);

uint8_t *mstep_eth_mac(int pin);
uint8_t *mstep_eth_ip(int pin);
uint8_t *mstep_eth_gw(int pin);
uint8_t *mstep_eth_mask(int pin);
void dm9051_irqlines_proc(void);
#endif

void cpin_poweron_reset(void);
uint8_t cspi_read_reg(uint8_t reg, int pin);
void cspi_read_regnx(uint8_t reg, uint8_t length, uint8_t *buf, int pin);
void cspi_write_reg(uint8_t reg, uint8_t val, int pin);
void cspi_write_regnx(uint8_t reg, uint8_t length, uint8_t *buf, int pin);
uint8_t cspi_read_mem2x(int pin);
void cspi_read_mem(u8 *buf, u16 len, int pin);
void cspi_write_mem(u8 *buf, u16 len, int pin);

//void mstep_next_net_index(void);
// char *mstep_conf_cpu_spi_ethernet(void);
char *mstep_conf_cpu_spi_ethernet(uint8_t pin);
// char *mstep_spi_conf_name(void);
char *mstep_spi_conf_name(uint8_t pin);

void exint_menable(nvic_priority_group_type priority); //void dm9051_board_irq_enable(void);

#if 1 //lw_config
#define DM9051_Poweron_Reset	cpin_poweron_reset
#define DM9051_Read_Reg			cspi_read_reg
#define DM9051_Read_Regnx 	cspi_read_regnx
#define DM9051_Write_Reg		cspi_write_reg
#define DM9051_Write_Regnx  cspi_write_regnx
#define DM9051_Read_Mem2X		cspi_read_mem2x
#define DM9051_Read_Mem			cspi_read_mem
#define DM9051_Write_Mem		cspi_write_mem
#endif

/* dm9051 delay times procedures */
void dm_delay_us(uint32_t nus);
void dm_delay_ms(uint16_t nms);

// char *mstep_conf_info(void);
char *mstep_conf_info(uint8_t pin);
// char *mstep_conf_cpu_cs_ethernet(void);
char *mstep_conf_cpu_cs_ethernet(uint8_t pin);

char *mstep_conf_type(void);
//int mstep_conf_spi_count(void);

int mstep_dm9051_index(void);

#endif //__DM9051_ENV_H
