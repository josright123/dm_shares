#include "dm9051opts.h"
#include "dm9051_lw.h"
#include "dm9051_lw_cspi.h"
#include "dm9051_lw_debug.h"

#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))

//static
void dm9051_link_to_hexdump(const void *buffer, size_t len) {
//	#undef printf
//	#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_HEX_DUMP, SEMA_ON, NULL, (fmt, ##__VA_ARGS__))
	#undef printf
	#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	if (dm9051_link_log_reset_hexdump(buffer, len)) {
		uint16_t rwpa_w, mdra_ingress;
		impl_read_rx_pointers(&rwpa_w, &mdra_ingress);
		printf("  rwpa %04x / igrss %04x\r\n", /*rx_modle_count[RX_ANY].allow_num,*/ rwpa_w, mdra_ingress);
	}
	#undef printf
	#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
//	#undef printf
//	#define printf(fmt, ...) TASK_DM9051_DEBUGF(0, SEMA_OFF, "[xx]", (fmt, ##__VA_ARGS__))
}

uint8_t rx_pointers_isr_show(char *headstr)
{
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))

	u16 rwpa_w, mdra_ingress;
	uint8_t rcr, isr;
	impl_read_rx_pointers(&rwpa_w, &mdra_ingress);
	rcr = DM9051_Read_Reg(DM9051_RCR);
	isr = DM9051_Read_Reg(DM9051_ISR);
	printf("%s[%d] %s rwpa %04x / igrss %04x\r\n",
			headstr, mstep_get_net_index(), mstep_spi_conf_name(), rwpa_w, mdra_ingress);
	printf("%s[%d] %s rcr 0x%02x isr 0x%02x\r\n",
			headstr, mstep_get_net_index(), mstep_spi_conf_name(), rcr, isr);
	return isr;

#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}