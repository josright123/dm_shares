/*
 * Copyright (c) 2023-2025 Davicom Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This DM9051 Driver for LWIP tcp/ip stack
 * First veryification with : AT32F415
 *
 * Author: Joseph CHANG <joseph_chang@davicom.com.tw>
 * Date: 20230411
 * Date: 20230428 (V3)
 * Date: 20240215 (Versions log please refer to 'dm9051_lw/info.txt')
 */

/* SPI master control is essential,
 * Do define spi specific definition, depend on your CPU's board support package library.
 * Here we had the definition header file with : "at32f415_spi.h"
 * for AT32F415 cpu.
 */
//#include <lwip/sys.h> //only for Lwip
#include "dm9051opts.h"
#include "dm9051_lw.h"
#include "dm9051_cboard.h"
//#include "dm9051_lw_cspi.h"

#include "dm9051_lw_log.h"
#include "dm9051_lw_debug.h"
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
#include "dm9051_bl_xxx.h"

#if freeRTOS
#include "lwip/sys.h"
#include "lwip/err.h"
#endif

void dm9051_pwron_reset(void)
{
	dm9051_reset_pulse();
}

int dm9051_init_setup(uint16_t *id)
{
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	*id = impl_read_chip_id();
	//*id = 0x9051;
	identify_chip_id(*id);
	return 1;
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

const uint8_t *hdlr_reset_process(const uint8_t *macaddr, enable_t en)
{
	dm9051_core_reset();

	if (en) { //As _dm9051_init's whole operations. Only for _CHMode
		macaddr = dm9051_start1(macaddr);
	}
	return macaddr;
}

//static 
void dm9051_core_reset(void)
{
	dm9051_ncr_reset(2);
	dm9051_phycore_on(250);
	dm9051_soft_default();
}

static int is_multicast_ether_addr(const uint8_t *addr)
{
	return 0x01 & addr[0];
}
static int is_zero_ether_addr(const uint8_t *addr)
{
	return (*(const uint16_t *)(addr + 0) |
			*(const uint16_t *)(addr + 2) |
			*(const uint16_t *)(addr + 4)) == 0;
}

static int is_valid_ether_addr(const uint8_t *addr)
{
	/* FF:FF:FF:FF:FF:FF is a multicast address so we don't need to
	 * explicitly check for it here. */
	return !is_multicast_ether_addr(addr) && !is_zero_ether_addr(addr);
}

static const uint8_t *internal_adr_logic(void)
{
	const uint8_t *adr;
	/*
	 * dm9051 start1 internal logic, i.e.
	 * Read par, if valid then it's from EEPROM
	 * otherwise, use Hard Core Candidate (hcc)
	 */

	//READ MAC_ADDR_LENGTH REGs
	uint8_t buff[6];
	impl_read_par(buff);

	if (is_valid_ether_addr(buff)) {
		adr = identify_eth_mac(buff, 0); /* Trick1 */
		printf(": EEPROM valid-mac[%d] ::: %02x %02x %02x %02x %02x %02x\r\n",
			mstep_get_net_index(), adr[0],adr[1],adr[2],adr[3],adr[4],adr[5]);
	} else {
		adr = identify_eth_mac(NULL, 0); /* Trick2 */
		printf(": Hard Core candidate-mac[%d] ::: %02x %02x %02x %02x %02x %02x\r\n",
			mstep_get_net_index(), adr[0],adr[1],adr[2],adr[3],adr[4],adr[5]);
	}
	return adr;
}

const uint8_t *dm9051_start1(const uint8_t *adr)
{
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))

	if (adr) {
		if (is_valid_ether_addr(adr)) {
			adr = identify_eth_mac(adr, 0); /* Trick0 */
		} else {
			printf(": APP DEF-INVAL-mac[%d] ::: %02x %02x %02x %02x %02x %02x (Invalid)\r\n",
				mstep_get_net_index(), adr[0],adr[1],adr[2],adr[3],adr[4],adr[5]);
			adr = internal_adr_logic();
		}
	}
	else
		adr = internal_adr_logic();

	impl_enable_mcu_irq(); //dm9051_extline_irq_enable(); //_dm9051_board_irq_enable(NVIC_PRIORITY_GROUP_0);
	dm9051_mac_adr(adr);
	dm9051_rx_mode();
	return adr;
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

//static 
void dm9051_mac_adr(const uint8_t *macadd) {
	impl_dm9051_set_par(macadd);
	//show_par();
}

//static 
void dm9051_rx_mode(void)
{
	dm9051_set_mar();
	//show_mar();
	dm9051_set_recv();
}

//rx
uint16_t buff_rx(uint8_t *buff)
{
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))

	uint16_t rx_len = dm9051_rx_read(buff);

	return rx_len;

#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

//tx
void buff_tx(uint8_t *buf, uint16_t len)
{	
	dm9051_tx_write(buf, len);
	dm9051_tx_req();
}

//rst
uint16_t impl_dm9051_err_hdlr(char *errstr, int pincode, uint32_t invalue, uint8_t zerochk)
{
	uint8_t *macaddr;
//#undef printf
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	char buf[76];
	int space_size = 76, n = 0;
	if (zerochk && invalue == 0)
		return 0;

	#if 1
	n += snprintf(buf+n, space_size-n, "----- [.]");
	n += snprintf(buf+n, space_size-n, errstr, pincode, invalue);
	
	if (n >= 76)
		n = 75;
	buf[n] = 0;
	
	bannerline_log();
	printf("----- [.]");
	printf(errstr, pincode, invalue); //or "0x%02x"
	printf(buf);
	#endif

	macaddr = identified_eth_mac(); //identify_eth_mac(adr, 0);
	hdlr_reset_process(macaddr, DM_TRUE); //CHMODE opts
	
	//rx_pointers_isr_show("dm9051_err_hdlr");
	return 0;
//#undef printf
//#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}
