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

const uint8_t *identify_eth_mac(const uint8_t *macadr, int showflg);
#if DM9051OPTS_API
uint8_t *identify_tcpip_ip(uint8_t *ip4adr);
uint8_t *identify_tcpip_gw(uint8_t *ip4adr);
uint8_t *identify_tcpip_mask(uint8_t *ip4adr);
uint8_t *identified_eth_mac(void);
uint8_t *identified_tcpip_ip(void);
uint8_t *identified_tcpip_gw(void);
uint8_t *identified_tcpip_mask(void);

#define	mstep_eth_mac()		identified_eth_mac()
#define	mstep_eth_ip()		identified_tcpip_ip()
#define	mstep_eth_gw()		identified_tcpip_gw()
#define	mstep_eth_mask()	identified_tcpip_mask()

bmcrmode_t mstep_opts_bmcrmode(void);

#define PINCOD mstep_get_net_index()
#define pincod mstep_get_index()

void mstep_set_net_index(int i);
int mstep_get_net_index(void);

#endif

//void mstep_next_net_index(void);
char *mstep_conf_cpu_spi_ethernet(void);
char *mstep_spi_conf_name(void);
void dm9051_board_irq_enable(void); //void _dm9051_board_irq_enable(void);

#if 1 //lw_config
#define DM9051_Poweron_Reset	cpin_poweron_reset
#define DM9051_Read_Reg			cspi_read_reg
#define DM9051_Write_Reg		cspi_write_reg
#define DM9051_Read_Mem2X		cspi_read_mem2x
#define DM9051_Read_Mem			cspi_read_mem
#define DM9051_Write_Mem		cspi_write_mem
#endif

/* dm9051 delay times procedures */
void dm_delay_us(uint32_t nus);
void dm_delay_ms(uint16_t nms);

char *mstep_conf_info(void);
char *mstep_conf_cpu_cs_ethernet(void);
char *mstep_conf_type(void);
//int mstep_conf_spi_count(void);

int mstep_dm9051_index(void);

void dm9051_delay_in_core_process(uint16_t nms, char *zhead);

#endif //__DM9051_ENV_H
