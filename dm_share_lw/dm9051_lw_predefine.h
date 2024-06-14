/* 
 * dm9051_lw_predefine.h
 */
#ifndef __DM9051_LW_PPED_H
#define __DM9051_LW_PRED_H

#include "stdint.h"
#include <string.h>

/* Your MCU */
#undef _AT32F403Axx  //#define _AT32F403Axx
#undef _AT32F437xx   //#define _AT32F437xx
#undef _AT32F413xx   //#define _AT32F413xx

/* Your switch */
#undef freeRTOS_CONF
#undef user_CONF

#endif //__DM9051_LW_PRED_H
