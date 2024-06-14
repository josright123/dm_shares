/* 
 * dm9051_lw_userdefine.h
 */
#ifndef __DM9051_LW_USER_H
#define __DM9051_LW_USER_H

#include "stdint.h"
#include <string.h>

/* Your MCU */
//#define _AT32F403Axx
//#define _AT32F413xx
//#define _AT32F437xx
/* Select */
#define _AT32F437xx

/* Your switch */
//#define freeRTOS_CONF  	0
//#define user_CONF			0
/* On/off */
#define freeRTOS_CONF  		0
#define user_CONF			1

/* Yourselves */
#if user_CONF

/* undef defines */
#undef _AT32F437xx
#undef freeRTOS_CONF
/* your update */
#include "../dm9051_lw_usr.h"

#endif

#endif //__DM9051_LW_USER_H
