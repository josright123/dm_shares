/**
  **************************************************************************
  * @file     "_dm9051opts_.h"
  * @version  v1.0
  * @date     2024-02-27
  * @brief    header file of dm9051 options.
  **************************************************************************
  */
#ifndef __DM9051_OPTS_H
#define __DM9051_OPTS_H

/*
 * dm9051_declaration_support
 */
#define ETHERNET_COUNT_MAX						4 // Correspond to mcu target board's specification
#define ETHERNET_COUNT							1 //2 //1 //4 //2 //2 //3 //2 //#define get_eth_interfaces() ETH_COUNT

/* Sanity.
 */
#if (ETHERNET_COUNT > ETHERNET_COUNT_MAX)
#error "Please make sure that _ETHERNET_COUNT(config here) must less equal to _ETHERNET_COUNT_MAX"
#endif

#define	ETHERNET_INTERRUPT_MODE					1 // Please keep define to 1, INT or POLLING by _data.h

#define DM9051OPTS_API							1
#define DM9051OPTS_LOG_ENABLE					1 //0
#define DM9051_DEBUG_ENABLE						1 //0

/*
 * Stop if id not corrext!
 */
#define NON_CHIPID_STOP							1 //0 //0 // stop
#define VER_CHIPID_ONLY							0 //1 //0

#define DM_UNUSED_ARG(x)						(void)x

#include "../dm9051_lw_userdefine.h"

/* Yourselves */
#if user_CONF
  /* user update define */
  #include "../dm9051_lw_usr_default.h"
#else
  // - driver common default
  // #include "../dm9051_lw_driver_default.h"
#endif

#ifdef freeRTOS_CONF
#define freeRTOS											freeRTOS_CONF
#else
#define freeRTOS											0
#endif

#if freeRTOS

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#warning "freeRTOS is defined"
#else

#warning "freeRTOS is NOT defined"
#endif

/*
 * at32_cm4_device_support
 */
#if freeRTOS
typedef void (* dly_us_t)(uint32_t nus);
typedef void (* dly_ms_t)(uint32_t nms);
#else
typedef void (* dly_us_t)(uint32_t nus);
typedef void (* dly_ms_t)(uint16_t nms);
#endif

typedef struct dm_dly_st {
	dly_us_t	dly_us;
	dly_ms_t	dly_ms;
} dm_dly_t;

//#ifdef _AT32F437xx
//	#include "at32f435_437_board.h" //mcu's board
//	#include "at32f435_437_clock.h" //Also mcu's clock
//#elif defined (_AT32F415xx)
//	#include "at32f415_board.h" //mcu's board
//	#include "at32f415_board.h" //Also mcu's clock
//#elif defined (_AT32F413xx)
//	#include "at32f413_board.h" //mcu's board
//	#include "at32f413_board.h" //Also mcu's clock
//#elif defined (_AT32F403Axx) || defined (_AT32F403xx) || defined (_AT32F407xx)
//	#include "at32f403a_407_board.h" //mcu's board
//	#include "at32f403a_407_clock.h" //Also mcu's clock
//#else
//	/*
//		#error "opts board to be define"
//		While NOT include the mcu-definition in the program code, in advance, previously.
//		Add your board's board & clock header files here! Refer to above cases.

//		Usually, Cn find the expected included files below in main.c
//	*/
//	#error "Opps, Opts board to be define  \r\n \
//		While NOT include the mcu-definition in the program code, in advance, previously. \r\n \
//		Usually, Project find the expected included files below in Options for Target > C/C++ > Preprocessor Definitions > Add > _AT32F4xxxx \r\n \
//		ex: _AT32F407xx,USE_STDPERIPH_DRIVER,AT_START_F407_V1 \r\n \
//		ex: _AT32F437xx,USE_STDPERIPH_DRIVER,AT_START_F437_V1 \r\n \ "
//#endif

/*
 * dm9051 include files : assembly
 */
typedef enum {
  DM_FALSE = 0,
  DM_TRUE = !DM_FALSE,
} enable_t;

typedef enum {
  CS_EACH = 0,
  CS_LONG,
} csmode_t;

typedef enum {
  BMCR_RST_DEFAULT = 0,
  BMCR_FORCE_100MF,
  BMCR_AUTO_NEG,
} bmcrmode_t;

void cboard_conf_configuration(void); //(uint8_t specified);
void cspi_read_regs(uint8_t reg, uint8_t *buf, uint16_t len, csmode_t csmode);
uint8_t cspi_read_reg(uint8_t reg);
void cspi_write_regs(uint8_t reg, uint8_t *buf, uint16_t len, csmode_t csmode);
void cspi_write_reg(uint8_t reg, uint8_t val);
uint8_t cspi_read_mem2x(void);
void cspi_read_mem(uint8_t *buf, uint16_t len);
void cspi_write_mem(uint8_t *buf, uint16_t len);

// - type 0, definitions
#define MAC_ADDR_LENGTH                  		(6)
#define ADDR_LENGTH                      		(4)

// - type 0, typedef
typedef uint8_t mac_t[MAC_ADDR_LENGTH];
typedef uint8_t ip_t[ADDR_LENGTH];

#define DM_TYPE		0
#include "dm_types1.h"

//.#include "dm9051spiopts.h"
//.#include "dm9051_lw_conf.h"

/*
 * dm9051_debug_mode selection
 */
//.#include "dm9051_lw_log.h"

//tobe dm9051_api.c api
//tobe dm9051opts.c internal
//
// [This _HELLO_DRIVER_INTERNAL compiler option, is for a diagnostic purpose while the program code is to use this dm9051_lw driver.]
// [Must be 1, for dm9051_lw driver operating.]
// [Set to 0, for the program code to observ the using APIs of dm9051_lw driver, before add the dm9051_lw driver implement files.]
//
#define HELLO_DRIVER_INTERNAL					1 //To support for being called by the program code from outside this dm9051_lw driver.

void GpioDisplay(void);
void ethcnt_ifdiplay(void);
uint8_t rx_pointers_isr_show(char *headstr);
//void first_log_clear(int i);
void first_log_init(void);
uint8_t first_log_get(int i);

#define PTR_SPIDEV(pin) /* new */ \
	&devconf[pin]

#define FIELD_SPIDEV(field) \
	/*devconf[pin_code].field */ \
	devconf[0].field

//enum {
//	SINGLE_TRANS,
//	MULTI_TRANS,
//};

void dm9051_board_counts_display(const char *project_name);

#if DM9051OPTS_API
void dm9051_mac_ip_display(char *head);
#else
#define dm9051_mac_ip_display(head)
#endif

#if DM9051OPTS_API
void dm9051_opts_iomod_etc(void);
#endif

//------------------
typedef void (* voidpin_t)(int i);
typedef void (* voidtx_t)(int pin, uint8_t *buf, uint16_t len);
void ETH_COUNT_VOIDFN(voidpin_t f); //internal, voidfn_dual
void ETH_COUNT_VOIDTX(voidtx_t pinfunc, uint8_t *buf, uint16_t len); //internal, voidtx_dual

//------------------
#define CPIN_PA08	0
#define CPIN_PB05	1

void spi_add(void);
void intr_add(void);
void cpin_rst_add(void);
void cpin_gpio_add(int cpin);

void cpin_poweron_reset(void);
void cpin_gpio_lo(int cpin);
void cpin_gpio_hi(int cpin);

void interface_all_add(int pin);

#define TO_ADD_CODE_LATER_BACK	0

#define DM9051_BOARD_CFG(expression, msg, handler) do { if ((expression)) { \
  printf("%s", msg); \
  (handler);}} while(0)

#if freeRTOS
	#define DM9051_MUTEX_CFG(expression, msg, handler) do { if ((expression)) { \
	  printf("%s", msg); \
	  (handler);}} while(0)
	#define DM9051_MUTEX_OPS(expression, handler) do { if ((expression)) { \
	  (handler);}} while(0)
#else
	#define DM9051_MUTEX_CFG(expression, msg, handler)
	#define DM9051_MUTEX_OPS(expression, handler)
#endif

#endif //__DM9051_OPTS_H
