/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __DM9051_LW_DEBUG_H
#define __DM9051_LW_DEBUG_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stdio.h"
// #include <stdint.h>

/**
 * @file dm9051_lw.h
 * @brief Header file for the DM9051 driver in the lwIP TCP/IP stack.
 *
 * This file contains the definitions and macros used for debugging the DM9051 driver.
 * It provides options to enable or disable debugging messages based on different debug levels.
 */
//#ifndef DM9051_DRIVER_DIAG
#define DM9051_DRIVER_DIAG(x) do {printf x;} while(0)
//#else
//#define DM9051_DRIVER_DIAG(x)
//#endif
//#define DM9051_DRIVER_DIAG1(x) do {printf x;} while(0)

#define DM9051_DRIVER_DEBUG

#ifdef DM9051_DRIVER_DEBUG
	#define DM9051_LW_CONF            0  //1
  #define DM9051_LW_LOG               0 //1

  #define DM9051_TRACE_DEBUG_OFF      0
  #define DM9051_TRACE_DEBUG_ON       1


	#define DM9051_DBG_ON              0x01
	#define DM9051_DBG_OFF             0x00
	#define DM9051_DBG_TYPES_ON        0x01
	#define DM9051_DBG_TYPES_OFF       0x00
	#define DM9051_DBG_MIN_LEVEL       0x00
	#define DM9051_DBG_LEVEL           0x03

	/**
	 * @brief Macro for printing debug messages.
	 *
	 * This macro is used to print debug messages based on the specified debug level.
	 * It checks if the debug flag is enabled, the debug types are enabled, and the debug level is within the minimum level.
	 * If all conditions are met, it calls the DM9051_DRIVER_DIAG macro to print the debug message.
	 *
	 * @param debug The debug flag.
	 * @param message The debug message to be printed.
	 */
	#define DM9051_DEBUGF(debug, message) do { \
			if ( \
				((debug) & DM9051_DBG_ON) && \
				((debug) & DM9051_DBG_TYPES_ON) && \
				(DM9051_DBG_LEVEL >= DM9051_DBG_MIN_LEVEL)) { \
				DM9051_DRIVER_DIAG(message); \
			}} while(0)
#else
	#define DM9051_DEBUGF(debug, message)
#endif

// example usage: DM9051_DEBUGF(DM9051_DBG_ON, ("dm9051_lw: %s: %d\n", __func__, __LINE__));
// example usage: DM9051_DEBUGF(DM9051_LW_CONF, ("dm9051_lw: %s: %d\n", __func__, __LINE__));


#ifdef __cplusplus
}
#endif

#endif //__DM9051_LW_DEBUG_H
