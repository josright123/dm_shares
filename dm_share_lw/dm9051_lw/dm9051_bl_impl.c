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
#if 0 //only for Lwip
#include <lwip/sys.h> //.temp
#endif
#include "dm9051opts.h"
#include "dm9051_lw.h"
#include "dm9051_lw_cspi.h"
#include "dm9051_lw_debug.h"
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))

//static
uint16_t impl_read_chip_id(void) {
	u8 buff[2];
	cspi_read_regs(DM9051_PIDL, buff, 2, CS_EACH);
	return buff[0] | buff[1] << 8;
}

//static
void impl_read_rx_pointers(u16 *rwpa_wt, u16 *mdra_rd) {
	*rwpa_wt = (uint32_t)DM9051_Read_Reg(0x24) | (uint32_t)DM9051_Read_Reg(0x25) << 8; //DM9051_RWPAL
	*mdra_rd = (uint32_t)DM9051_Read_Reg(0x74) | (uint32_t)DM9051_Read_Reg(0x75) << 8; //DM9051_MRRL;
}

//static
uint16_t impl_phy_read(uint16_t uReg)
{
	int w = 0;
	u16 uData;

#if 1
  //_CH390
  //if (uReg == PHY_STATUS_REG)
  //{
	//dm9051_phycore_on(0); //if (uReg == PHY_STATUS_REG)
  //}
  if (DM_GET_FIELD(uint16_t, read_chip_id) == 0x9151 && uReg == PHY_STATUS_REG)
	dm9051_phycore_on(0);
#endif

	DM9051_Write_Reg(DM9051_EPAR, DM9051_PHY | uReg);
	DM9051_Write_Reg(DM9051_EPCR, 0xc);
	dm_delay_us(1);
	while(DM9051_Read_Reg(DM9051_EPCR) & 0x1) {
		dm_delay_us(1);
		if (++w >= 500) //5
			break;
	} //Wait complete

	DM9051_Write_Reg(DM9051_EPCR, 0x0);
	uData = (DM9051_Read_Reg(DM9051_EPDRH) << 8) | DM9051_Read_Reg(DM9051_EPDRL);

	#if 0
	if (uReg == PHY_STATUS_REG) {
		if (uData  & PHY_LINKED_BIT)
			dm9051_set_flags(lw_flag, DM9051_FLAG_LINK_UP);
		else
			dm9051_clear_flags(lw_flag, DM9051_FLAG_LINK_UP);
	}
	#endif

	return uData;
}

#define DM9051_RX_BREAK(expression, handler) do { if ((expression)) { \
  handler;}} while(0)


/*static*/
u16 impl_dm9051_err_hdlr(char *errstr, int pincode, u32 invalue, u8 zerochk)
{
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	char buf[76];
	int space_size = 76, n = 0;
	if (zerochk && invalue == 0)
		return 0; //.printf(": NoError as %u\r\n", valuecode);

#if 0
	printf(errstr, pincode, invalue); //or "0x%02x"
#else
	n += snprintf(buf+n, space_size-n, "----- [.]");
	n += snprintf(buf+n, space_size-n, errstr, pincode, invalue);

	if (n >= 76) n = 75;
	buf[n] = 0;

	bannerline_log();
	printf("----- [.]");
	printf(errstr, pincode, invalue); //or "0x%02x"
	printf(buf);
#endif

	hdlr_reset_process(mstep_eth_mac(), OPT_CONFIRM(hdlr_confrecv)); //CH390 opts

#if 1
//	rx_pointer_show("dm9051_err_hdlr");
//	rx_isr_show("dm9051_err_hdlr");
	rx_pointers_isr_show("dm9051_err_hdlr");
#endif

	return 0;
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

/*static*/
u16 impl_dm9051_err_hdlr_01(char *errstr, int pincode, u32 invalue, u8 zerochk)
{
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	char buf[76];
	int space_size = 76, n = 0;
	if (zerochk && invalue == 0)
		return 0; //.printf(": NoError as %u\r\n", valuecode);

#if 0
	printf(errstr, pincode, invalue); //or "0x%02x"
#else
	n += snprintf(buf+n, space_size-n, "----- [.]");
	n += snprintf(buf+n, space_size-n, errstr, pincode, invalue);

	if (n >= 76) n = 75;
	buf[n] = 0;

	bannerline_log();
	printf("----- [.]");
	printf(errstr, pincode, invalue); //or "0x%02x"
	printf(buf);
#endif
	hdlr_reset_process(mstep_eth_mac(), OPT_CONFIRM(hdlr_confrecv)); //CH390 opts

#if 1
//	rx_pointer_show("dm9051_err_hdlr");
//	rx_isr_show("dm9051_err_hdlr");
	rx_pointers_isr_show("dm9051_err_hdlr");
#endif

	return 0xffff;
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

static uint16_t buff_rx_01(uint8_t *buff)
{
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	uint16_t rx_len = 0;
	uint8_t rxbyte, rx_status;
	uint8_t ReceiveData[4];

	rxbyte = DM9051_Read_Mem2X(); //DM9051_Read_Rxb(); //DM9051_Read_Reg(DM9051_MRCMDX);
	//DM9051_RXB_Basic(rxbyte); //(todo) Need sevice case.
	buff[0] = rxbyte;
	buff[1] = rxbyte;
	buff[2] = rxbyte;
	buff[3] = rxbyte;
	DM9051_RX_BREAK((rxbyte != 0x01 && rxbyte != 0), return ev_rxb_01(rxbyte));
	DM9051_RX_BREAK((rxbyte == 0), return 0);

	DM9051_Read_Mem(ReceiveData, 4);
	DM9051_Write_Reg(DM9051_ISR, 0x80);

	rx_status = ReceiveData[1];
	rx_len = ReceiveData[2] + (ReceiveData[3] << 8);

	//instead of : err_hdlr("_dm9051f rx_status error : 0x%02x\r\n", rx_status, 0)
	memcpy(buff, ReceiveData, 4);
	DM9051_RX_BREAK((rx_status & 0xbf), printf("ev_status: %02x %02x %02x %02x\r\n",
			ReceiveData[0],ReceiveData[1],ReceiveData[2],ReceiveData[3]));

	#if 	1		// 1= app check rx_len
	DM9051_RX_BREAK((rx_status & 0xbf), return ev_status_01(rx_status));
	#else
	if (rx_status & 0xbf) {
		// return ev_status_01(rx_status);
		ev_status_01(rx_status);
		printf("rx_len = %u\r\n", rx_len);
		printf("\r\n");
		DM9051_Read_Mem(buff, rx_len);
		DM9051_Write_Reg(DM9051_ISR, 0x80);
		return 0;
	}
#endif

	//instead of : err_hdlr("_dm9051f rx_len error : %u\r\n", rx_len, 0));
	DM9051_RX_BREAK((rx_len > RX_POOL_BUFSIZE), return impl_dm9051_err_hdlr_01("_dm9051f[%d] rx_len error : %u\r\n", PINCOD, rx_len, 0));

	DM9051_Read_Mem(buff, rx_len);
	DM9051_Write_Reg(DM9051_ISR, 0x80);
	return rx_len;
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

static uint16_t buff_rx(uint8_t *buff)
{
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
	uint16_t rx_len = 0;
	uint8_t rxbyte, rx_status;
	uint8_t ReceiveData[4];

	rxbyte = DM9051_Read_Mem2X(); //DM9051_Read_Rxb(); //DM9051_Read_Reg(DM9051_MRCMDX);
	//DM9051_RXB_Basic(rxbyte); //(todo) Need sevice case.

	DM9051_RX_BREAK((rxbyte != 0x01 && rxbyte != 0), return ev_rxb(rxbyte));
	DM9051_RX_BREAK((rxbyte == 0), return 0);

	DM9051_Read_Mem(ReceiveData, 4);
	DM9051_Write_Reg(DM9051_ISR, 0x80);

	rx_status = ReceiveData[1];
	rx_len = ReceiveData[2] + (ReceiveData[3] << 8);

	//instead of : err_hdlr("_dm9051f rx_status error : 0x%02x\r\n", rx_status, 0)
	DM9051_RX_BREAK((rx_status & 0xbf), printf("ev_status: %02x %02x %02x %02x\r\n",
			ReceiveData[0],ReceiveData[1],ReceiveData[2],ReceiveData[3]));
	DM9051_RX_BREAK((rx_status & 0xbf), return ev_status(rx_status));
	//instead of : err_hdlr("_dm9051f rx_len error : %u\r\n", rx_len, 0));
	DM9051_RX_BREAK((rx_len > RX_POOL_BUFSIZE), return impl_dm9051_err_hdlr("_dm9051f[%d] rx_len error : %u\r\n", PINCOD, rx_len, 0));

	DM9051_Read_Mem(buff, rx_len);
	DM9051_Write_Reg(DM9051_ISR, 0x80);
	return rx_len;
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

uint16_t impl_dm9051_rx1(uint8_t *buff)
{
//	if (!dm9051_get_flags()) {
//		dm9051_unlink_inc();
//		dm9051_bmsr_update();

//		return 0;
//	}
	return buff_rx(buff);
}

uint16_t impl_dm9051_rx1_01(uint8_t *buff)
{
//	if (!dm9051_get_flags()) {
//		dm9051_unlink_inc();
//		dm9051_bmsr_update();

//		return 0;
//	}
	return buff_rx_01(buff);
}

#define DM9051_TX_DELAY(expression, handler) do { if ((expression)) { \
  handler;}} while(0)

void impl_dm9051_tx1(uint8_t *buf, uint16_t len)
{
	DM9051_Write_Reg(DM9051_TXPLL, len & 0xff);
	DM9051_Write_Reg(DM9051_TXPLH, (len >> 8) & 0xff);
	DM9051_Write_Mem(buf, len);
	DM9051_Write_Reg(DM9051_TCR, TCR_TXREQ); /* Cleared after TX complete */
	DM9051_TX_DELAY((DM9051_Read_Reg(DM9051_TCR) & TCR_TXREQ), dm_delay_us(5));
}

