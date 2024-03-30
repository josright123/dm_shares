//
// dm9051opts.c
//
#include "dm9051opts.h"
#include "dm9051_lw.h"
#include "dm9051_lw_debug.h"

#if 	freeRTOS
// Declare the mutex handle globally so it can be accessed in different tasks
SemaphoreHandle_t mutex_lw_print;
#endif

void dm9051_debug_init(void)
{
  #if 	freeRTOS
  // Create a mutex
  mutex_lw_print = xSemaphoreCreateMutex();
  if (mutex_lw_print == NULL) {
    printf("Mutex could not be created.\r\n");
    while(1); // Handle the error, e.g., by entering an infinite loop
  }
  #endif

}
