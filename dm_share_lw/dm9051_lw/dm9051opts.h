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

#include "stdint.h"
#include <string.h>
#include "../dm9051_lw_usr.h"

/*
 * dm9051_declaration_support
 */
#define ETHERNET_COUNT_MAX 	4 // Correspond to mcu target board's specification
#define ETHERNET_COUNT 			4		 // 2 //2 //4 //2 //2 //3 //2 //#define get_eth_interfaces() ETH_COUNT
#define freeRTOS 						0
#define freeRTOS_LOCK_SPI_MUTEX 1

#define _AT32F403Axx
// #define _AT32F437xx

#ifdef freeRTOS_CONF
#undef freeRTOS
#define freeRTOS freeRTOS_CONF
#endif

/* Sanity.
 */
#if (ETHERNET_COUNT > ETHERNET_COUNT_MAX)
#error "Please make sure that _ETHERNET_COUNT(config here) must less equal to _ETHERNET_COUNT_MAX"
#endif

#define DM9051OPTS_API 					1
#define DM9051OPTS_LOG_ENABLE 	1 // 0
#define DM9051_DEBUG_ENABLE 		1		// 0

/*
 * Stop if id not corrext!
 */
#define NON_CHIPID_STOP 1 // 0 //0 // stop
#define VER_CHIPID_ONLY 0 // 1 //0

/*
 * at32_cm4_device_support
 */
#if freeRTOS
// #ifndef freeRTOS_CONF
#warning "freeRTOS is defined"
// #endif

#include "FreeRTOS.h"
#include "task.h"
typedef void (*dly_us_t)(uint32_t nus);
typedef void (*dly_ms_t)(uint32_t nms);
static void uvTaskDelay(const TickType_t xTicksToDelay)
{
	vTaskDelay((xTicksToDelay + 999) / 1000);
}

// 對於微秒級的延遲，您可能需要使用忙等待迴圈或硬體支援的計時器。
// void delay_us(uint32_t us)
// {
//     // 假設系統時鐘為 100MHz，請根據您的硬體調整此值
//     const uint32_t us_ticks = 100;
//     uint32_t i;
//     for(i = 0; i < us * us_ticks; i++)
//     {
//         __NOP();
//     }
// }
#else
typedef void (*dly_us_t)(uint32_t nus);
typedef void (*dly_ms_t)(uint32_t nms);
#endif

typedef struct dm_dly_st
{
	dly_us_t dly_us;
	dly_ms_t dly_ms;
} dm_dly_t;

#ifdef _AT32F437xx
#include "at32f435_437_board.h" //mcu's board
#include "at32f435_437_clock.h" //Also mcu's clock
#elif defined(_AT32F415xx)
#include "at32f415_board.h" //mcu's board
#include "at32f415_board.h" //Also mcu's clock
#elif defined(_AT32F413xx)
#include "at32f413_board.h" //mcu's board
#include "at32f413_board.h" //Also mcu's clock
#elif defined(_AT32F403Axx) || defined(_AT32F403xx) || defined(_AT32F407xx)
#include "at32f403a_407_board.h" //mcu's board
#include "at32f403a_407_clock.h" //Also mcu's clock
#else
/*
	#error "opts board to be define"
	While NOT include the mcu-definition in the program code, in advance, previously.
	Add your board's board & clock header files here! Refer to above cases.

	Usually, Cn find the expected included files below in main.c
*/
// #include "at32f435_437_board.h" //mcu's board
// #include "at32f435_437_clock.h" //Also mcu's clock
#error "Opts board to be define  \r\n \
		While NOT include the mcu-definition in the program code, in advance, previously. \r\n \
		Usually, Project find the expected included files below in Options for Target > C/C++ > Preprocessor Definitions > Add > _AT32F4xxxx \r\n \
		ex: _AT32F407xx,USE_STDPERIPH_DRIVER,AT_START_F407_V1 \r\n \
		ex: _AT32F437xx,USE_STDPERIPH_DRIVER,AT_START_F437_V1 \r\n \ "
#endif

static const dm_dly_t dmf = {
#if freeRTOS
		uvTaskDelay, // here assign, define system's delay us function
		vTaskDelay,	 // here assign, define system's delay ms function
#else
		delay_us, // here assign, define system's delay us function
		delay_ms, // here assign, define system's delay ms function
#endif
};

/*
 * dm9051 include files : assembly
 */
typedef enum
{
	DM_FALSE = 0,
	DM_TRUE = !DM_FALSE,
} enable_t;

typedef enum
{
	CS_EACH = 0,
	CS_LONG,
} csmode_t;

typedef enum
{
	BMCR_RST_DEFAULT = 0,
	BMCR_FORCE_100MF,
	BMCR_AUTO_NEG,
} bmcrmode_t;

// void cpin_poweron_reset(void);
// void cspi_read_regs(uint8_t reg, u8 *buf, u16 len, csmode_t csmode);
// uint8_t cspi_read_reg(uint8_t reg);
// void cspi_write_reg(uint8_t reg, uint8_t val);
// uint8_t cspi_read_mem2x(void);
// void cspi_read_mem(u8 *buf, u16 len);
// void cspi_write_mem(u8 *buf, u16 len);
// 加入 pin 參數
void cspi_read_regs(uint8_t reg, u8 *buf, u16 len, csmode_t csmode, int pin);
uint8_t cspi_read_reg(uint8_t reg, int pin);
void cspi_write_reg(uint8_t reg, uint8_t val, int pin);
uint8_t cspi_read_mem2x(int pin);
void cspi_read_mem(u8 *buf, u16 len, int pin);
void cspi_write_mem(u8 *buf, u16 len, int pin);


#define DM_TYPE 0
#include "dm_types.h"

// include ["_dm9051opts_.h"]
#include "dm9051spiopts.h"
#include "dm9051_lw_conf.h"

/*
 * dm9051_debug_mode selection
 */
#include "dm9051_lw_log.h"

// tobe dm9051_api.c api
// tobe dm9051opts.c internal
//
//  [This _HELLO_DRIVER_INTERNAL compiler option, is for a diagnostic purpose while the program code is to use this dm9051_lw driver.]
//  [Must be 1, for dm9051_lw driver operating.]
//  [Set to 0, for the program code to observ the using APIs of dm9051_lw driver, before add the dm9051_lw driver implement files.]
//
#define HELLO_DRIVER_INTERNAL 1 // To support for being called by the program code from outside this dm9051_lw driver.

#if HELLO_DRIVER_INTERNAL

#if 0
	/* sub-extern *//* SET and GET */
	/*#define SG_FUNCTION(rtype, field) \
		rtype dm9051opts_get##rtype##field(void); \
		void dm9051opts_set##rtype##field(rtype value)*/

	/* sub *//* SET and GET */
	/*#define DECL_SG_FUNCTION(rtype, field) \
	rtype dm9051opts_get##rtype##field(void) { \
		return dm9051optsex[mstep_get_net_index()].##field; \
	} \
	void dm9051opts_set##rtype##field(rtype value) { \
		dm9051optsex[mstep_get_net_index()].##field = value; \
	}*/
#endif

#endif // HELLO_DRIVER_INTERNAL

//------------------

// dm9051opts.c
void GpioDisplay(void);

void ethcnt_ifdiplay(void);

// void first_log_clear(int i);
void first_log_init(void);
u8 first_log_get(int i);

//------------------

#define DM_UNUSED_ARG(x) (void)x

// enum {
//	SINGLE_TRANS,
//	MULTI_TRANS,
// };
#if DM9051OPTS_API
// typedef uint16_t (* trn_conn_t)(int i);
// int TRANS_DUAL(trn_conn_t f); //return : found id number
void dm9051_opts_display(void);

// typedef uint16_t (* trn_nonconn_t)(uint8_t *macadr); //typedef void (* trans_t)(void *arg);
// int TRANS_NONDUAL(trn_nonconn_t f, uint8_t *arg); //return : chip id
#else
// #define TRANS_DUAL(f)
#define dm9051_opts_display()

/* dm9051opts.c
 * , uint8_t trans_type
 * == SINGLE_TRANS,
 *  return : chip id
 */
// int TRANS_NONDUAL(trn_nonconn_t f, uint8_t *arg) {
//   return f(arg);
// }
// #define TRANS_NONDUAL(f,arg)
#endif

#if DM9051OPTS_API
#if DM9051OPTS_LOG_ENABLE
void dm9051_opts_iomod_etc(void);

#else

#define dm9051_opts_iomod_etc()
#endif
#endif

//------------------
typedef void (*voidpin_t)(int i);
typedef void (*voidtx_t)(int pin, uint8_t *buf, uint16_t len);
void ETH_COUNT_VOIDFN(voidpin_t f);																	 // internal, voidfn_dual
void ETH_COUNT_VOIDTX(voidtx_t pinfunc, uint8_t *buf, uint16_t len); // internal, voidtx_dual

//------------------

#define TO_ADD_CODE_LATER_BACK 0

#endif //__DM9051_OPTS_H
