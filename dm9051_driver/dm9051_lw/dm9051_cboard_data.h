#ifndef __DM9051_BOARD_IMPL_H__
#define __DM9051_BOARD_IMPL_H__

#include "dm9051_lw_debug.h"
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))

//[dm9051_lw_cint_data.h]
int intr_gpio_mptr(void) {
	return intr_pointer() ? 1 : 0;
}

//int is_dm9051_board_irq(void)
//{
//  const struct extscfg_st *pexint_set;
//
//	pexint_set = NULL;
//  if (intr_pointer())
//    pexint_set = (const struct extscfg_st *)intr_data_scfg(); //exint_scfg_ptr();
//
//	printf(".is_dm9051_irq %s\r\n", pexint_set ? "(INT)" : "(POLLING)");
//
//  return pexint_set ? 1 : 0;
//}

void ETH_COUNT_VOIDFN(voidpin_t f) {
  int i;
  for (i = 0; i < ETHERNET_COUNT; i++) {
	  //.mstep_set_net_index(i);
	  f(i);
  }
}

#if 0
void ETH_COUNT_VOIDTX(voidtx_t pinfunc, uint8_t *buf, uint16_t len) {
  int i;
  for (i = 0; i < ETHERNET_COUNT; i++) {
	  mstep_set_net_index(i);
	  pinfunc(i, buf, len);
  }
}
#endif

/* DataObj.devconf[] and DataObj.intrconf[] are
 * waiting to be used!
 */
typedef struct {
	const spi_dev_t *devconf[BOARD_SPI_COUNT];
	const struct modscfg_st *intrconf[BOARD_SPI_COUNT];
} conf_list_t;

/* DataObj.devconf[] and DataObj.intrconf[] are
 * waiting to be used!
 */
static conf_list_t DataObj;

//static 
uint8_t Is_DataObj_Exist(uint32_t exint_line) {
	int pin;
	for (pin = 0; pin < ETHERNET_COUNT; pin++) {
		if (dm9051_irq_exint_line(pin) == exint_line)
			return 1;
	}
	return 0;
}

/* ret: EXINT_LINE_0 ~ EXINT_LINE_22
 *      if (!DataObj.intrconf[pin]) return 0
 */
uint32_t dm9051_irq_exint_line(int pin)
{
	return DataObj.intrconf[pin] ? DataObj.intrconf[pin]->extend->extline.extline : 0;
}

//static 
int Get_DataObj_Pin(uint32_t exint_line) {
	int pin;
	for (pin = 0; pin < ETHERNET_COUNT; pin++) {
		if (dm9051_irq_exint_line(pin) == exint_line)
			return pin;
	}
	return -1; //Not the pin
}

/* ret: pincode
 */
int dm9051_irq_pincode(uint32_t exint_line)
{
	if (Is_DataObj_Exist(exint_line))
		return Get_DataObj_Pin(exint_line);
	
	//.list_extline(exint_line);
	return 0;
}

void DataObj_store(int pin) {
	DataObj.devconf[pin] = &devconf[pin];
	DataObj.intrconf[pin] = intr_pointer(); //Can it in case NULL ok ?
}

//.

int de_enum(uint32_t data) {
	int n;
	
	for (n = 0; n < 16; n++)
		if (data & (1 << n))
			return n;
	return 999; //? unknow
}

int de_pin(const gpio_t *gpio) {
	return de_enum(gpio->pin); //check uint16_t as to uint32_t is possible.
}

//[list]
void list_extline(uint32_t exint_line) {
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	int pin;
	if (exint_line)
		printf("#Enter 0x%06x= EXINT_LINE_%d\r\n", exint_line, de_enum(exint_line));
		
	for (pin = 0; pin < ETHERNET_COUNT; pin++)
		printf(".List%d: 0x%06x= EXINT_LINE_%d\r\n", pin,
				dm9051_irq_exint_line(pin), de_enum(dm9051_irq_exint_line(pin)));
		
	if (exint_line)
		printf("#Exit 0x%06x= EXINT_LINE_%d, NOT found!\r\n", exint_line, de_enum(exint_line));
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

#endif //__DM9051_BOARD_IMPL_H__
