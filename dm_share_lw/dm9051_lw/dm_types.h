//.#ifndef __DM_TYPES_H
//.#define __DM_TYPES_H

//dm_types.h

#define DM_TYPES_GET_CALL(mtype, field)			dm9051opts_get_##mtype##field()
#define DM_TYPES_GET_FUNC(mtype, field)			dm9051opts_get_##mtype##field(void)

#define DM_TYPES_SET_CALL(mtype, field, setval)	dm9051opts_set_##mtype##field(setval)
#define DM_TYPES_SET_FUNC(mtype, field, setval)	dm9051opts_set_##mtype##field(const mtype setval)

#define DM_TYPES_GETDESC_CALL(mtype, field)		dm9051opts_desc_##field()
#define DM_TYPES_GETDESC_FUNC(mtype, field)		dm9051opts_desc_##field(void)
		
#if DM_TYPE == 0

	// - type 0, definitions
	#define MAC_ADDR_LENGTH                  		(6)
	#define ADDR_LENGTH                      		(4)

	typedef uint8_t mac_t[MAC_ADDR_LENGTH];
	typedef uint8_t ip_t[ADDR_LENGTH];

	// - type 0, call in the application program

	#define DM_GET_FIELD(mtype, field)			DM_TYPES_GET_CALL(mtype, field)			/*= dm9051opts_get_##mtype##field()*/
	#define DM_GET_DESC(mtype, field)			DM_TYPES_GETDESC_CALL(mtype, field)		/*dm9051opts_desc##field()*/
	#define DM_SET_FIELD(mtype, field, setval)	DM_TYPES_SET_CALL(mtype, field, setval)	/*dm9051opts_set_##mtype##field(setval)*/
		
//	#define get_testplanlog(name)			DM_GET_FIELD(enable_t, name)

	#define OPT_U8(name)					DM_GET_FIELD(uint8_t, name) //appcall
	#define OPT_CS(name)					DM_GET_FIELD(csmode_t, name) //appcall
	
//	#define OPT_BMCR(name)					DM_GET_FIELD(bmcrmode_t, name) //appcall
	#define OPT_CONFIRM(name)				DM_GET_FIELD(enable_t, name) //appcall (define isonlybytemode(), or OPT_CONFIRM(onlybytemode))

	// - type 0, extern API

	#undef DM_MACRO
	#define DM_MACRO(rtype, mtype, field) \
		rtype DM_TYPES_GET_FUNC(mtype, field); \
		char *DM_TYPES_GETDESC_FUNC(mtype, field); /*dm9051opts_desc##field(void);*/ \
		void DM_TYPES_SET_FUNC(mtype, field, setval); /*dm9051opts_set_##mtype##field(const mtype setval);*/

DM_MACRO(uint8_t*,	mac_t, final_mac) //(uint8_t*, mac6_t, mac)
DM_MACRO(uint8_t*,	ip_t, ip) //(uint8_t*, ip4_t, ip)
DM_MACRO(uint8_t*,	ip_t, gw) //(uint8_t*, ip4_t, gw)
DM_MACRO(uint8_t*,	ip_t, mask) //(uint8_t*, ip4_t, mask)

#elif DM_TYPE == 1

	// - type 1, field

	#undef DM_MACRO
	#define DM_MACRO(rtype, mtype, field)  \
		mtype field; \
		char *desc##field;

DM_MACRO(void,	mac_t, final_mac) //(uint8_t*, mac6_t, mac)
DM_MACRO(void,	ip_t, ip) //(uint8_t*, ip4_t, ip)
DM_MACRO(void,	ip_t, gw) //(uint8_t*, ip4_t, gw)
DM_MACRO(void,	ip_t, mask) //(uint8_t*, ip4_t, mask)

#elif DM_TYPE == 2

	// - type 2, implement

	#undef DM_MACRO
	#define DM_MACRO(rtype, mtype, field)  \
		rtype DM_TYPES_GET_FUNC(mtype, field) { \
			return (rtype) dm9051optsex[mstep_get_net_index()].##field; \
		} \
		char *DM_TYPES_GETDESC_FUNC(mtype, field) { \
			return dm9051optsex[mstep_get_net_index()].desc##field; \
		} \
		void DM_TYPES_SET_FUNC(mtype, field, setval) { /* extended-set-data */ \
			dm9051optsex[mstep_get_net_index()].##field = setval; \
		}
	#undef DM_RMACRO
	#define DM_RMACRO(rtype, mtype, field, adr_len)  \
		rtype DM_TYPES_GET_FUNC(mtype, field) { \
			return (rtype) dm9051optsex[mstep_get_net_index()].##field; \
		} \
		char *DM_TYPES_GETDESC_FUNC(mtype, field) { \
			return dm9051optsex[mstep_get_net_index()].desc##field; \
		} \
		void DM_TYPES_SET_FUNC(mtype, field, setval) { /* extended-set-data */ \
			/*dm9051optsex[mstep_get_net_index()].##field = val;*/ \
			memcpy(dm9051optsex[mstep_get_net_index()].##field, setval, adr_len); \
		}

//DM_RMACRO(void,	mac_t, mac)
//DM_RMACRO(void,	ip_t, ip)
//DM_RMACRO(void,	ip_t, gw)
//DM_RMACRO(void,	ip_t, mask)
DM_RMACRO(uint8_t*,	mac_t, final_mac, MAC_ADDR_LENGTH)
DM_RMACRO(uint8_t*,	ip_t, ip, ADDR_LENGTH)
DM_RMACRO(uint8_t*,	ip_t, gw, ADDR_LENGTH)
DM_RMACRO(uint8_t*,	ip_t, mask, ADDR_LENGTH)

#endif

DM_MACRO(uint16_t,	uint16_t, read_chip_id)

DM_MACRO(enable_t,	enable_t, test_plan_log) //.SG_FUNCTION(enable_t, test_plan_log);
DM_MACRO(uint8_t,	uint8_t, iomode)
DM_MACRO(csmode_t,	csmode_t, csmode)
DM_MACRO(bmcrmode_t, bmcrmode_t, bmcrmode)
DM_MACRO(uint8_t,	uint8_t, promismode)
DM_MACRO(enable_t,	enable_t, rxtypemode)
DM_MACRO(enable_t,	enable_t, rxmode_checksum_offload)
DM_MACRO(enable_t,	enable_t, flowcontrolmode)
DM_MACRO(enable_t,	enable_t, onlybytemode)
DM_MACRO(uint8_t,	uint8_t, hdir_x2ms)
DM_MACRO(enable_t,	enable_t, hdlr_confrecv)
DM_MACRO(enable_t,	enable_t, tx_endbit)
DM_MACRO(enable_t,	enable_t, generic_core_rst)

#undef DM_TYPE

//.#endif //__DM_TYPES_H
