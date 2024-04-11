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
	// Define the macro definitions
	#define DM_GET_FIELD_PIN(mtype, field, pin) \
	dm9051opts_pin_##mtype##field(pin)

	#define DM_GET_DESC_PIN(mtype, field, pin) \
	dm9051opts_pin_desc_##field(pin)

	#define DM_SET_FIELD_PIN(mtype, field, val, pin) \
	dm9051opts_pin_set_##mtype##field(pin, val)

	#define get_testplanlog_PIN(name, pin)		DM_GET_FIELD_PIN(enable_t, name, pin)
	#define OPT_U8_PIN(name, pin)						  DM_GET_FIELD_PIN(uint8_t, name, pin) //appcall
	#define OPT_CS_PIN(name, pin)							DM_GET_FIELD_PIN(csmode_t, name, pin) //appcall
	#define OPT_BMCR_PIN(name, pin)						DM_GET_FIELD_PIN(bmcrmode_t, name, pin) //appcall
	#define OPT_CONFIRM_PIN(name, pin)				DM_GET_FIELD_PIN(enable_t, name, pin)
	// - type 0, extern API
	#undef DM_MACRO_PIN
	#define DM_MACRO_PIN(rtype, mtype, field) \
		rtype dm9051opts_pin_##mtype##field(int pin); \
		char *dm9051opts_pin_desc_##field(int pin) ; \
		void dm9051opts_pin_set_##mtype##field(int pin, const mtype val);

		DM_MACRO_PIN(uint8_t*,	mac_t, mac) 	//(uint8_t*, mac6_t, mac)
		DM_MACRO_PIN(uint8_t*,	ip_t, ip) 		//(uint8_t*, ip4_t, ip)
		DM_MACRO_PIN(uint8_t*,	ip_t, gw) 		//(uint8_t*, ip4_t, gw)
		DM_MACRO_PIN(uint8_t*,	ip_t, mask) 	//(uint8_t*, ip4_t, mask)

#elif DM_TYPE == 1
	// - type 1, field

//	#undef DM_MACRO
//	#define DM_MACRO(rtype, mtype, field)  \
//		mtype field; \
//		char *desc##field;

	#undef DM_MACRO_PIN
	#define DM_MACRO_PIN(rtype, mtype, field) \
		mtype field; \
		char *desc_##field;

	DM_MACRO_PIN(void,	mac_t, mac) 	//(uint8_t*, mac6_t, mac)
	DM_MACRO_PIN(void,	ip_t, ip) 		//(uint8_t*, ip4_t, ip)
	DM_MACRO_PIN(void,	ip_t, gw) 		//(uint8_t*, ip4_t, gw)
	DM_MACRO_PIN(void,	ip_t, mask) 	//(uint8_t*, ip4_t, mask)

#elif DM_TYPE == 2
	// - type 2, implement

//	#undef DM_MACRO
//	#define DM_MACRO(rtype, mtype, field)  \
//		rtype dm9051opts_##mtype##field(void) { \
//			return (rtype) dm9051optsex[mstep_get_net_index()].##field; \
//		} \
//		char *dm9051opts_desc##field(void) { \
//			return dm9051optsex[mstep_get_net_index()].desc##field##; \
//		} \
//		void dm9051opts_set_##mtype##field(const mtype val) { /* extended-set-data */ \
//			dm9051optsex[mstep_get_net_index()].##field = val; \
//		}
		// Test001 code is OK.
	#undef DM_MACRO_PIN
	#define DM_MACRO_PIN(rtype, mtype, field) \
		rtype dm9051opts_pin_##mtype##field(int pin) { \
				return (rtype) dm9051optsex[pin].##field; \
		} \
		char *dm9051opts_pin_desc_##field(int pin) { \
				return dm9051optsex[pin].desc_##field##; \
		} \
		void dm9051opts_pin_set_##mtype##field(int pin, const mtype val) { \
				dm9051optsex[pin].##field = val; \
		}

	#undef DM_RMACRO_PIN
	#define DM_RMACRO_PIN(rtype, mtype, field, adr_len)  \
		rtype dm9051opts_pin_##mtype##field(int pin) { \
			return (rtype) dm9051optsex[pin].##field; \
		} \
		char *dm9051opts_pin_desc_##field(int pin) { \
			return dm9051optsex[pin].desc_##field##; \
		} \
		void dm9051opts_pin_set_##mtype##field(int pin, const mtype val) { \
			memcpy(dm9051optsex[pin].##field, val, adr_len); \
		}
	DM_RMACRO_PIN(uint8_t*,	mac_t, mac, MAC_ADDR_LENGTH)
	DM_RMACRO_PIN(uint8_t*,	ip_t, ip, ADDR_LENGTH)
	DM_RMACRO_PIN(uint8_t*,	ip_t, gw, ADDR_LENGTH)
	DM_RMACRO_PIN(uint8_t*,	ip_t, mask, ADDR_LENGTH)

#endif

DM_MACRO_PIN(uint16_t,	uint16_t, read_chip_id)
DM_MACRO_PIN(enable_t,	enable_t, test_plan_log)
DM_MACRO_PIN(uint8_t,	uint8_t, iomode)
DM_MACRO_PIN(csmode_t,	csmode_t, csmode)
DM_MACRO_PIN(bmcrmode_t, bmcrmode_t, bmcrmode)
DM_MACRO_PIN(uint8_t,	uint8_t, promismode)
DM_MACRO_PIN(enable_t,	enable_t, rxtypemode)
DM_MACRO_PIN(enable_t,	enable_t, rxmode_checksum_offload)
DM_MACRO_PIN(enable_t,	enable_t, flowcontrolmode)
DM_MACRO_PIN(enable_t,	enable_t, onlybytemode)
DM_MACRO_PIN(uint8_t,	uint8_t, hdir_x2ms)
DM_MACRO_PIN(enable_t,	enable_t, hdlr_confrecv)
DM_MACRO_PIN(enable_t,	enable_t, tx_endbit)
DM_MACRO_PIN(enable_t,	enable_t, generic_core_rst)

#undef DM_TYPE

//.#endif //__DM_TYPES_H
