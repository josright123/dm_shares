//.#ifndef __DM_TYPES_H
//.#define __DM_TYPES_H

// pronoun - type 0, in the application
#define DM_TYPES_GET_CALL(mtype, field)			dm9051opts_get_##mtype##field()
#define DM_GET_FIELD							DM_TYPES_GET_CALL
//#define DM_GET_FIELD							DM_TYPES_GET_CALL	
//#define DM_GET_FIELD(mtype, field)			DM_TYPES_GET_CALL(mtype, field)

#define DM_TYPES_SET_CALL(mtype, field, setval)	dm9051opts_set_##mtype##field(setval)
#define DM_SET_FIELD							DM_TYPES_SET_CALL
//#define DM_SET_FIELD							DM_TYPES_SET_CALL
//#define DM_SET_FIELD(mtype, field, setval)	DM_TYPES_SET_CALL(mtype, field, setval)

//#define DM_TYPES_GETDESC_CALL(mtype, field)	dm9051opts_desc_##field()
//#define DM_GET_DESC							DM_TYPES_GETDESC_CALL
//#define DM_GET_DESC(mtype, field, setval)		DM_TYPES_GETDESC_CALL(mtype, field, setval)

// macro - type 0, call in the application program
#define DM_TYPES_GET_FUNC(mtype, field)			dm9051opts_get_##mtype##field(void)
#define DM_TYPES_SET_FUNC(mtype, field, setval)	dm9051opts_set_##mtype##field(const mtype setval)
#define DM_TYPES_GETDESC_FUNC(mtype, field)		dm9051opts_desc_##field(void)

#if DM_TYPE == 0
	// - type 0, extern API
	#undef DM_MACRO
	#define DM_MACRO(rtype, mtype, field) \
		rtype DM_TYPES_GET_FUNC(mtype, field); \
		char *DM_TYPES_GETDESC_FUNC(mtype, field); \
		void DM_TYPES_SET_FUNC(mtype, field, setval);
#elif DM_TYPE == 1
	// - type 1, field
	#undef DM_MACRO
	#define DM_MACRO(rtype, mtype, field)  \
		mtype field; \
		char *desc##field;
#elif DM_TYPE == 2
	// - type 2, implement
	#undef DM_MACRO
	#define DM_MACRO(rtype, mtype, field)  \
		rtype DM_TYPES_GET_FUNC(mtype, field) { \
			return (rtype) dm9051optsex[mstep_get_net_index()].field; \
		} \
		char *DM_TYPES_GETDESC_FUNC(mtype, field) { \
			return dm9051optsex[mstep_get_net_index()].desc##field; \
		} \
		void DM_TYPES_SET_FUNC(mtype, field, setval) { \
			dm9051optsex[mstep_get_net_index()].field = setval; \
		}
	#undef DM_RMACRO
	#define DM_RMACRO(rtype, mtype, field, adr_len)  \
		rtype DM_TYPES_GET_FUNC(mtype, field) { \
			return (rtype) dm9051optsex[mstep_get_net_index()].field; \
		} \
		char *DM_TYPES_GETDESC_FUNC(mtype, field) { \
			return dm9051optsex[mstep_get_net_index()].desc##field; \
		} \
		void DM_TYPES_SET_FUNC(mtype, field, setval) { \
			memcpy(dm9051optsex[mstep_get_net_index()].field, setval, adr_len); \
		}
#endif

#if DM_TYPE == 0
	DM_MACRO(uint8_t*,	mac_t, final_mac)
	DM_MACRO(uint8_t*,	ip_t, ip)
	DM_MACRO(uint8_t*,	ip_t, gw)
	DM_MACRO(uint8_t*,	ip_t, mask)
#elif DM_TYPE == 1		
	DM_MACRO(uint8_t*,	mac_t, final_mac)
	DM_MACRO(uint8_t*,	ip_t, ip)
	DM_MACRO(uint8_t*,	ip_t, gw)
	DM_MACRO(uint8_t*,	ip_t, mask)
#elif DM_TYPE == 2
	DM_RMACRO(uint8_t*,	mac_t, final_mac, MAC_ADDR_LENGTH)
	DM_RMACRO(uint8_t*,	ip_t, ip, ADDR_LENGTH)
	DM_RMACRO(uint8_t*,	ip_t, gw, ADDR_LENGTH)
	DM_RMACRO(uint8_t*,	ip_t, mask, ADDR_LENGTH)
#endif

DM_MACRO(uint16_t,	uint16_t, read_chip_id)
DM_MACRO(uint16_t,	uint16_t, irqst)

#if 0
//_dm_types1.h
//DM_MACRO(enable_t,	enable_t, test_plan_log)
//DM_MACRO(uint8_t,		uint8_t, iomode)
//DM_MACRO(csmode_t,	csmode_t, csmode)
//DM_MACRO(bmcrmode_t, 	bmcrmode_t, bmcrmode)
//DM_MACRO(uint8_t,		uint8_t, promismode)
//DM_MACRO(enable_t,	enable_t, rxtypemode)
//DM_MACRO(enable_t,	enable_t, rxmode_checksum_offload)
//DM_MACRO(enable_t,	enable_t, flowcontrolmode)
//DM_MACRO(enable_t,	enable_t, onlybytemode)
//DM_MACRO(uint8_t,		uint8_t, hdir_x2ms)
//DM_MACRO(enable_t,	enable_t, hdlr_confrecv)
//DM_MACRO(enable_t,	enable_t, tx_endbit)
//DM_MACRO(enable_t,	enable_t, generic_core_rst)
#endif

#undef DM_TYPE
//.#endif //__DM_TYPES_H
