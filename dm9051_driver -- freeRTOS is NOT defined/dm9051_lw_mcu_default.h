#ifndef __DM9051_LW_MCU_H
#define __DM9051_LW_MCU_H

#define _AT32F437xx
//#define _AT32F437xx

#ifdef _AT32F437xx
	#include "at32f435_437_board.h" //mcu's board
	#include "at32f435_437_clock.h" //Also mcu's clock
#elif defined (_AT32F415xx)
	#include "at32f415_board.h" //mcu's board
	#include "at32f415_board.h" //Also mcu's clock
#elif defined (_AT32F413xx)
	#include "at32f413_board.h" //mcu's board
	#include "at32f413_board.h" //Also mcu's clock
#elif defined (_AT32F403Axx) || defined (_AT32F403xx) || defined (_AT32F407xx)
	#include "at32f403a_407_board.h" //mcu's board
	#include "at32f403a_407_clock.h" //Also mcu's clock
#else
	/*
		#error "opts board to be define"
		While NOT include the mcu-definition in the program code, in advance, previously.
		Add your board's board & clock header files here! Refer to above cases.

		Usually, Cn find the expected included files below in main.c
	*/
	#error "Opps, Opts board to be define  \r\n \
		While NOT include the mcu-definition in the program code, in advance, previously. \r\n \
		Usually, Project find the expected included files below in Options for Target > C/C++ > Preprocessor Definitions > Add > _AT32F4xxxx \r\n \
		ex: _AT32F407xx,USE_STDPERIPH_DRIVER,AT_START_F407_V1 \r\n \
		ex: _AT32F437xx,USE_STDPERIPH_DRIVER,AT_START_F437_V1 \r\n \ "
#endif

#endif //__DM9051_LW_MCU_H
