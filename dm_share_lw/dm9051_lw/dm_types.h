//.#ifndef __DM_TYPES_H
//.#define __DM_TYPES_H

//dm_types.h

#if DM_TYPE == 0
	/* call */
	#define DM_FUNC(rtype, field) \
		dm9051opts_##rtype##field()
	#define DM_DESC(rtype, field) /* to be usage */ \
		dm9051opts_desc##field()

	#define DM_SET_FIELD(rtype, field, val) /* DM_SET_FUNC(ty,fi,val), extended-set-data */ \
		dm9051opts_set_##rtype##field(val)
		
	//#define set_testplanlog				IS_SET_INSTEAD(enable_t, test_plan_log)
	//#define get_testplanlog				IS_GET_INSTEAD(enable_t, test_plan_log)=
	#define get_testplanlog(name)			DM_FUNC(enable_t, name)
	#define OPT_U8(name)					DM_FUNC(uint8_t, name) //appcall
	#define OPT_CS(name)					DM_FUNC(csmode_t, name) //appcall
	#define OPT_BMCR(name)					DM_FUNC(bmcrmode_t, name) //appcall
	#define OPT_CONFIRM(name)				DM_FUNC(enable_t, name) //appcall (define isonlybytemode(), or OPT_CONFIRM(onlybytemode))

	/* extern API */
	#undef DM_MACRO
	#define DM_MACRO(rtype, field) \
		rtype dm9051opts_##rtype##field(void); \
		char *dm9051opts_desc##field(void); \
		void dm9051opts_set_##rtype##field(rtype val); /* extended-set-data */

#elif DM_TYPE == 1
	/* field */
	#undef DM_MACRO
	#define DM_MACRO(rtype,field) \
		rtype field; \
		char *desc##field;

#elif DM_TYPE == 2
	/* implement */
	#undef DM_MACRO
	#define DM_MACRO(rtype, field) \
		rtype dm9051opts_##rtype##field(void) { \
			return dm9051optsex[mstep_get_net_index()].##field; \
		} \
		char *dm9051opts_desc##field(void) { \
			return dm9051optsex[mstep_get_net_index()].desc##field##; \
		} \
		void dm9051opts_set_##rtype##field(rtype val) { /* extended-set-data */ \
			dm9051optsex[mstep_get_net_index()].##field = val; \
		}
#endif

DM_MACRO(uint16_t, read_chip_id)

DM_MACRO(enable_t, test_plan_log) //.SG_FUNCTION(enable_t, test_plan_log);
DM_MACRO(uint8_t, iomode)
DM_MACRO(csmode_t, csmode)
DM_MACRO(bmcrmode_t, bmcrmode)
DM_MACRO(uint8_t, promismode)
DM_MACRO(enable_t, rxtypemode)
DM_MACRO(enable_t, rxmode_checksum_offload)
DM_MACRO(enable_t, flowcontrolmode)
DM_MACRO(enable_t, onlybytemode)
DM_MACRO(uint8_t, hdir_x2ms)
DM_MACRO(enable_t, hdlr_confrecv)
DM_MACRO(enable_t, tx_endbit)
DM_MACRO(enable_t, generic_core_rst)

#undef DM_TYPE

//.#endif //__DM_TYPES_H
