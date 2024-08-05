#include "dm9051opts.h"
//#include "dm9051_lw.h"
#include "dm9051_cboard.h"
#include "../dm9051_lw_mcu_default.h"
#include "dm9051_lw_conf_types.h"

#include "dm9051_lw_cspi.h"
#include "dm9051_lw_cint.h"
#include "dm9051_lw_crst_cgpio.h"

#include "dm9051_lw_cspi_data.h"
#include "dm9051_lw_cint_data.h"
#include "dm9051_lw_crst_cgpio_data.h"
#include "dm9051_cboard_data.h"

#include "../freertos_tasks_debug.h"
#undef printf
#define printf(fmt, ...) TASK_DM9051_DEBUGF(TASK_SEMAPHORE_NETCONF_ON, /*SEMA_OFF*/ SEMA_ON, /*"[D]"*/ "", (fmt, ##__VA_ARGS__))

#define kkmin(a, b) (a < b) ? a : b

static int printf_space_init(size_t tlen) {
	char cspace[12];
	int n = sprintf(cspace, "rxlen %d", tlen);
	printf(cspace);
	return n;
}
			
static void printf_space(int n) {
	while (n--)
		printf("%c", ' ');
}

void sprint_hex_dump0(int head_space, int titledn, char *prefix_str,
		    size_t tlen, int rowsize, const void *buf, int seg_start, size_t len, /*int useflg*/ int cast_lf) //, int must, int dgroup
{
//#undef printf
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	//if (useflg) {
		int si, se, titlec = 0;
		int i, linelen, remaining = len; //hs, const eth_class_t *ec = &eclass[10];
		int nspace = 0;
		
		(void) head_space;
		//(void) tlen;

		si = seg_start;
		se = seg_start + len;
		for (i = si; i < se; i += rowsize) {
			//unsigned
			char linebuf[(12 * 3) + (3 * 16) + 1 + 32]; //here!

			linelen = kkmin(remaining, rowsize);
			remaining -= rowsize;
			do {
				const uint8_t *ptr = buf;
				int j;
				//unsigned char *
				int nb = 0;
				for (j=0; j< linelen && (size_t) nb < sizeof(linebuf); j++) {
					if (j && !(j%8))
						nb += snprintf(linebuf+nb, sizeof(linebuf)-nb, " ");
					if (((rowsize>>1)!=8) && !(j%(rowsize>>1)))
						nb += snprintf(linebuf+nb, sizeof(linebuf)-nb, " ");

					nb += snprintf(linebuf+nb, sizeof(linebuf)-nb, "%02x ", *(ptr + i + j));
				}
			} while(0);

#if 1
			//printf("%d", head_space);
			if (!nspace)
				nspace = printf_space_init(tlen);
			else
				printf_space(nspace);
#endif
#if 0
			hs = head_space;
			while(hs--)
#endif
				printf(" ");

			if (prefix_str) {
				printf("(%s) %.3x %s", prefix_str, i, linebuf); //"%s", ec->str //CHECK (XXX >> )
				while (titledn) {
					titledn--;
					prefix_str[titlec++] = ' ';
				}
			} else {
				printf("(dm9 xfer) %.3x %s", i, linebuf); //"%s", ec->str
			}

			if ((i+rowsize) < se)
				printf("\r\n");
			else {
				if (cast_lf)
					printf("\r\n");
#if 0
				if (IS_UDP) {
					//ptr
					#if 0
					size_t ulen = tlen; // larger for with 4-bytes CRC
					ulen = UIP_LLH_LEN;
					ulen += HTONS(UDPBUF->udplen) - 8;
					ulen += sizeof(struct uip_udpip_hdr); // correct for without 4-bytes CRC (htons)

					if (cast_lf)
						printf("\r\n");

					printf(" ..SrcIP %d.%d.%d.%d", (IPBUF->srcipaddr[0] >> 0) & 0xff, (IPBUF->srcipaddr[0] >> 8),
						(IPBUF->srcipaddr[1] >> 0) & 0xff, (IPBUF->srcipaddr[1] >> 8));
					printf("  DestIP %d.%d.%d.%d", (IPBUF->destipaddr[0] >> 0) & 0xff, (IPBUF->destipaddr[0] >> 8),
						(IPBUF->destipaddr[1] >> 0) & 0xff, (IPBUF->destipaddr[1] >> 8));
					printf("  Len %d", ulen);
					printf("  (%5d -> %d Len %d)", UDPBUF->srcport, UDPBUF->destport, HTONS(UDPBUF->udplen) - 8);
					printf("\r\n");
					#endif
				}
				if (IS_TCP) {
					size_t ulen = tlen; // larger for with 4-bytes CRC
					printf(" ..SrcIP %d.%d.%d.%d", (IPBUF->srcipaddr[0] >> 0) & 0xff, (IPBUF->srcipaddr[0] >> 8),
						(IPBUF->srcipaddr[1] >> 0) & 0xff, (IPBUF->srcipaddr[1] >> 8));
					printf("  DestIP %d.%d.%d.%d", (IPBUF->destipaddr[0] >> 0) & 0xff, (IPBUF->destipaddr[0] >> 8),
						(IPBUF->destipaddr[1] >> 0) & 0xff, (IPBUF->destipaddr[1] >> 8));
					printf("  Len %d", ulen);
					
					if (TCPBUF->flags == 0x18)
						printf("  (%5d -> %d) flags %02x (PSH, ACK)", HTONS(TCPBUF->srcport), HTONS(TCPBUF->destport), TCPBUF->flags);
					else
						printf("  (%5d -> %d) flags %02x", HTONS(TCPBUF->srcport), HTONS(TCPBUF->destport), TCPBUF->flags);
					
					printf("\r\n");
				}
#endif
				//else
				//	printf("\r\n");
			}
		}
	//}
//#undef printf
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}
