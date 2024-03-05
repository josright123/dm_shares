//.#ifndef __DM_TYPES_H
//.#define __DM_TYPES_H

//dm_types.h

#if DM_TYPE == 0

	// - type 0, definitions
	#define MAC_ADDR_LENGTH                  		(6)
	#define ADDR_LENGTH                      		(4)

	typedef uint8_t mac_t[MAC_ADDR_LENGTH];
	typedef uint8_t ip_t[ADDR_LENGTH];

	// - type 0, call

	#define DM_FUNC(mtype, field) \
		dm9051opts_##mtype##field()
	#define DM_DESC(mtype, field) /* to be usage */ \
		dm9051opts_desc##field()
	#define DM_SET_FIELD(mtype, field, val) /* DM_SET_FUNC(ty,fi,val), extended-set-data */ \
		dm9051opts_set_##mtype##field(val)
		
	//#define set_testplanlog				IS_SET_INSTEAD(enable_t, test_plan_log)
	//#define get_testplanlog				IS_GET_INSTEAD(enable_t, test_plan_log)=
	#define get_testplanlog(name)			DM_FUNC(enable_t, name)
	#define OPT_U8(name)					DM_FUNC(uint8_t, name) //appcall
	#define OPT_CS(name)					DM_FUNC(csmode_t, name) //appcall
	#define OPT_BMCR(name)					DM_FUNC(bmcrmode_t, name) //appcall
	#define OPT_CONFIRM(name)				DM_FUNC(enable_t, name) //appcall (define isonlybytemode(), or OPT_CONFIRM(onlybytemode))

	// - type 0, extern API

	#undef DM_MACRO
	#define DM_MACRO(rtype, mtype, field) \
		rtype dm9051opts_##mtype##field(void); \
		char *dm9051opts_desc##field(void); \
		void dm9051opts_set_##mtype##field(const mtype val); /* extended-set-data */

DM_MACRO(uint8_t*,	mac_t, mac) //(uint8_t*, mac6_t, mac)
DM_MACRO(uint8_t*,	ip_t, ip) //(uint8_t*, ip4_t, ip)
DM_MACRO(uint8_t*,	ip_t, gw) //(uint8_t*, ip4_t, gw)
DM_MACRO(uint8_t*,	ip_t, mask) //(uint8_t*, ip4_t, mask)

#elif DM_TYPE == 1

	// - type 1, field

	#undef DM_MACRO
	#define DM_MACRO(rtype, mtype, field)  \
		mtype field; \
		char *desc##field;

DM_MACRO(void,	mac_t, mac) //(uint8_t*, mac6_t, mac)
DM_MACRO(void,	ip_t, ip) //(uint8_t*, ip4_t, ip)
DM_MACRO(void,	ip_t, gw) //(uint8_t*, ip4_t, gw)
DM_MACRO(void,	ip_t, mask) //(uint8_t*, ip4_t, mask)

#elif DM_TYPE == 2

	// - type 2, implement

	#undef DM_MACRO
	#define DM_MACRO(rtype, mtype, field)  \
		rtype dm9051opts_##mtype##field(void) { \
			return (rtype) dm9051optsex[mstep_get_net_index()].##field; \
		} \
		char *dm9051opts_desc##field(void) { \
			return dm9051optsex[mstep_get_net_index()].desc##field##; \
		} \
		void dm9051opts_set_##mtype##field(const mtype val) { /* extended-set-data */ \
			dm9051optsex[mstep_get_net_index()].##field = val; \
		}
	#undef DM_RMACRO
	#define DM_RMACRO(rtype, mtype, field, adr_len)  \
		rtype dm9051opts_##mtype##field(void) { \
			return dm9051optsex[mstep_get_net_index()].##field; \
		} \
		char *dm9051opts_desc##field(void) { \
			return dm9051optsex[mstep_get_net_index()].desc##field##; \
		} \
		void dm9051opts_set_##mtype##field(const mtype val) { /* extended-set-data */ \
			/*dm9051optsex[mstep_get_net_index()].##field = val;*/ \
			memcpy(dm9051optsex[mstep_get_net_index()].##field, val, adr_len); \
		}

//DM_RMACRO(void,	mac_t, mac)
//DM_RMACRO(void,	ip_t, ip)
//DM_RMACRO(void,	ip_t, gw)
//DM_RMACRO(void,	ip_t, mask)
DM_RMACRO(uint8_t*,	mac_t, mac, MAC_ADDR_LENGTH)
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
