#ifndef __DM9051_SYS_ARCH__
#define __DM9051_SYS_ARCH__

xSemaphoreHandle mutex_create_start(void);
void sys_mutex_lock_start(xSemaphoreHandle *sema_p);
void sys_mutex_unlock_end(xSemaphoreHandle *sema_p);

#if freeRTOS
/** The global semaphore to lock the stack. */
//extern sys_mutex_t lock_dm9051_core;
//# define LOCK_TCPIP_COREx()     sys_mutex_lock(&lock_dm9051_core)
//# define ULOCK_TCPIP_COREx()   sys_mutex_unlock(&lock_dm9051_core)
//# else
//# define LOCK_TCPIP_COREx()	//empty test
//# define ULOCK_TCPIP_COREx()	//empty test

#endif
#endif //__DM9051_SYS_ARCH__
