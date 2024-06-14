//
// dm9051opts.c
//
#include "dm9051opts.h"
#include "dm9051_lw.h"

u8 gfirst_log[ETHERNET_COUNT];

void first_log_init(void)
{
	int i;
	for (i = 0; i < ETHERNET_COUNT; i++)
		gfirst_log[i] = 1; //Can know the first time reset the dm9051 chip!
}

u8 first_log_get(int i)
{
	u8 if_log = 
		gfirst_log[i];
	gfirst_log[i] = 0; //first_log_clear(i);
	return if_log;
}

//void first_log_clear(int i)
//{
//	gfirst_log[i] = 0;
//}

/* , uint8_t trans_type
 * == MULTI_TRANS,
 *  return : found id number
 */
//int TRANS_DUAL(trn_conn_t f) {
//  int i;
//  int nID = 0;
//  uint16_t id;
//  for (i = 0; i < ETHERNET_COUNT; i++) {
	//.mstep_set_net_index(i); + //dm9051_init(_mstep_eth_mac()); 
//	id = f(i); //= drviver_init(i)
//	if (check_chip_id(id))
//	 nID++;
//  }
//return nID;
//}

//voidfn_dual
void ETH_COUNT_VOIDFN(voidpin_t f) {
  int i;
  for (i = 0; i < ETHERNET_COUNT; i++) {
	  mstep_set_net_index(i);
	  f(i);
  }
}

//voidtx_dual
void ETH_COUNT_VOIDTX(voidtx_t pinfunc, uint8_t *buf, uint16_t len) {
  int i;
  for (i = 0; i < ETHERNET_COUNT; i++) {
	  mstep_set_net_index(i);
	  pinfunc(i, buf, len);
  }
}
