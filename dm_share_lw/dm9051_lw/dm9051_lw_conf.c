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
#include "dm9051_lw_cint.h"
#include "dm9051_lw_debug.h"
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))

#include "dm9051_lw_conf_data.h" //1.yicheng 2.later than 'board_printf'

//#ifdef AT32F437xx
//#include "dm9051_lw_conf_at437x2spi.h" //1.yicheng
//#else
//#include "dm9051_lw_conf_at437x2spi.h" //or "dm9051_lw_conf_at415x2spi.h"
//#endif

/*
In xxx_int.c, 

To have interrupt mode support function.

	void EXINT9_5_IRQHandler(void) {
	  dm9051_irqlines_proc(); //call the 'dm9051_irqlines_proc()',
	}
*/

//void dm9051_irqlines_proc(void)
//{
////  void xxethernetif_exint_proc(int i);
//  
//	  printf("dm9051_irqlines_proc()-ethernetif _exint _proc()\r\n");
//	  if (!exint_exister()) //[To be enum , e.g. intr_pack[i], if multi-cards]
//		return;
//  
//	  if (exint_flag_get(exint_extline()) != RESET) //if (exint_flag_get(EXINT_LINE_7) != RESET)
//	  {
//		#if _ETHERNET_INTERRUPT_MODE
//		#if 1
//		if (exint_extline() == EXINT_LINE_7)
//		{
//			int pin = 0;
//			if (exint_extline() == EXINT_LINE_7)
//				pin = 0;
//			
//			xxethernetif_exint_proc(pin);
//		}
//		#endif
//		#endif
//		exint_flag_clear(exint_extline()); //exint_flag_clear(EXINT_LINE_7);
//	  }
//}

/*********************************
 * dm9051 macro
 *********************************/
#define	board_printf(format, args...) //int board_printf(const char *format, args...) { return 0; }

/*********************************
 * dm9051 delay times procedures
 *********************************/
void dm_delay_us(uint32_t nus) {
//	void delay_us(uint32_t nus);
	board_printf("test %d ,because rxb %02x (is %d times)\r\n", rstccc, rxbyteee, timesss);
	dmf.dly_us(nus); //delay_us(nus);
}
void dm_delay_ms(uint16_t nms) {
//	void delay_ms(uint16_t nms);
	dmf.dly_ms(nms); //delay_ms(nms);
}

void rst_add(void)
{
  if (rst_pin_mexist()) {
	/* rst_gpio_ptr() should be correct pin define.
	 */
	printf("gpio_pin_config: RST-gpio %d\r\n", de_pin(rst_gpio_ptr())); //&gpio_wire_mi()//&rst_gpio_ptr()
	gpio_pin_config(rst_gpio_ptr(), GPIO_PULL_UP); //=(rst_gpio_ptr(_pinCode), GPIO_PULL_UP); //,GPIO_MODE_OUTPUT
  }
}

void interface_all_add(int pin)
{
	DM_UNUSED_ARG(pin);

	spi_add();
	rst_add();
	intr_add();
//	exint_line0_config();
	
//	exint_line4_config();
}

/*
 * pDevObj[] is
 * waiting to be used!
 */
//const spi_dev_t *pDevObj[BOARD_SPI_COUNT]; //To be used, in case 'pin_code' can be eliminated mass-quantity.

//void DevObj_store(int pin) {
//	pDevObj[pin] = PTR_SPIDEV(pin); //'pin_code'
//}

/*
 * DataObj.devconf[] and DataObj.intrconf[] are
 * waiting to be used!
 */
typedef struct {
	const spi_dev_t *devconf[BOARD_SPI_COUNT];
	const struct modscfg_st *intrconf[BOARD_SPI_COUNT];
} conf_list_t;

conf_list_t DataObj;

//Example:
// = {
//	{
//		&devconf[0], //devconf_at437_spi4("AT32F437", "sck/mi/mo/ pe2/pe5/pe6", "cs/ pe4"),
//		&devconf[1], //devconf_at437_spi2("AT32F437", "sck/mi/mo/ pd1/pc2/pd4", "cs/ pd0"),
//	},
//	{
//		&devconf_at437_intr_c7,
//		&devconf_at437_intr_a0,
//	},
//};
void DataObj_store(int pin) {
	DataObj.devconf[pin] = &devconf[pin]; //PTR_SPIDEV(pin); //'pin_code'
//	if (_intr_packPT)
	DataObj.intrconf[pin] = intr_pointer(); //instead of, ((const struct modscfg_st **)_intr_packPT)[pin];
							//Can it in case NULL ok ?
//	  DataObj.intrconf[pin] = &((struct modscfg_st *)intr_packPT)[pin];
}

uint32_t DataObj_EXINT_extline(int pin)
{
	return DataObj.intrconf[pin] ? DataObj.intrconf[pin]->extend->extline.extline : 0;
}

void LIST_EXTLINE(uint32_t exint_line)
{
	int pin;
	if (exint_line)
		printf("Enter exint_line 0x%06x\r\n", exint_line);
	else
		printf("Enter EXINT_LINE_NONE 0x%06x\r\n", exint_line);
		
	for (pin = 0; pin < ETHERNET_COUNT; pin++)
		printf("List%d = 0x%06x\r\n", pin, DataObj.intrconf[pin]->extend->extline.extline);
		
	if (exint_line)
		printf("Exit exint_line 0x%06x NOT found!\r\n", exint_line);
	else
		printf("Exit EXINT_LINE_NONE 0x%06x\r\n", exint_line);
}

int DataObj_EXINT_Pin(uint32_t exint_line)
{
	int pin;
	for (pin = 0; pin < ETHERNET_COUNT; pin++) {
		if (DataObj.intrconf[pin]->extend->extline.extline == exint_line)
			return pin;
	}
	
//	printf("Enter exint_line 0x%06x\r\n", exint_line);
//	for (pin = 0; pin < ETHERNET_COUNT; pin++)
//		printf("List%d = 0x%06x\r\n", pin, DataObj.intrconf[pin]->extend->extline.extline);
//	printf("Exit exint_line 0x%06x NOT found!\r\n", exint_line);
	LIST_EXTLINE(exint_line);

	return 0;
}

void board_conf_configuration(void)
{
  /*for (i = 0; i < n; i++) { //get_eth_interfaces()
	mstep_set_net_index(i);
	interface_all_add(i);
  }*/

//  ETH_COUNT_VOIDFN(DevObj_store);
  ETH_COUNT_VOIDFN(DataObj_store);
  LIST_EXTLINE(EXINT_LINE_NONE);

  ETH_COUNT_VOIDFN(interface_all_add); //voidfn_dual

  cpin_poweron_reset();
  dmf.dly_ms(30);
}

//void ethernet_interfaces_initialize(void)
//{
//  int i;
//  for (i = 0; i < _ETHERNET_COUNT; i++) {
//	mstep_set_net_index(i); //+
//	dm9051_init(_get_eth_mac());
//  }
//}

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

/*
 * Include user defined options first. Anything not defined in these files
 * will be set to standard values. Override anything you don't like!
 */
#if 0 //only for Lwip
#include "lwipopts.h"
#endif

#if freeRTOS_CONF && freeRTOS_ENABLE_MUTEX
int cspiSemaphoreDoOwn(int pntlog, char *headstr, SemaphoreHandle_t semaphore_hdlr);
void cspiSemaphoreDoYield(int pntlog, char *headstr, SemaphoreHandle_t semaphore_hdlr);

#define INIT_CSPI_CORE() \
	int rc;
	
#define LOCK_CSPI_CORE(log) \
	if (freeRTOS_ENABLE_MUTEX) \
		rc = cspiSemaphoreDoOwn(log, "empty", NULL);
#define UNLOCK_CSPI_CORE() \
	if (freeRTOS_ENABLE_MUTEX) \
		if (rc) cspiSemaphoreDoYield(0, "empty", NULL);
#else
#define INIT_CSPI_CORE()
#define LOCK_CSPI_CORE(log)
#define UNLOCK_CSPI_CORE()
#endif

//static
void dm9051_delay_in_core_process(uint16_t nms, char *zhead) //finally, dm9051_lw.c
{
	if (nms)
		printf(": dm9051_driver[%d] ::: %s delay %u ms.. : \r\n", mstep_get_net_index(), zhead, nms);
	if (nms)
	  dm_delay_ms(nms); //_delay_ms(nms); //from James' advice! to be determined with a reproduced test cases!!
}
