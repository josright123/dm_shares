//
// dm9051_lw_cint_data.h
// - intr configuration data
//

#ifdef AT32F437xx

//[CRM_SCFG_PERIPH_CLOCK] //essential
struct extscfg_st pe_c7 = {
	"enable SCFG, extline pc7",
	{ CRM_GPIOC_PERIPH_CLOCK, EXINT_LINE_7, EXINT9_5_IRQn, NVIC_PRIORITY_GROUP_0}, // correspond to and with PC7
};

//[CRM_SCFG_PERIPH_CLOCK] //essential
struct extscfg_st pe_a0 = {
	"enable SCFG, extline pa0",
	{ CRM_GPIOA_PERIPH_CLOCK, EXINT_LINE_0, EXINT0_IRQn, NVIC_PRIORITY_GROUP_4}, // correspond to and with PA0
};

//optional
gp_set_t gp_c7 = {
	"GPIO pc7",
	{GPIOC, GPIO_PINS_7,  CRM_GPIOC_PERIPH_CLOCK, &mode_input, }, //(PC7) INT-pin //GPIO_MODE_INPUT, GPIO_PINSRC_NULL, GPIO_MUX_NULL
};
gp_set_t gp_a0 = {
	"GPIO pa0",
	{GPIOA, GPIO_PINS_0,  CRM_GPIOA_PERIPH_CLOCK, &mode_input, }, //(PA0) INT-pin //GPIO_MODE_INPUT, GPIO_PINSRC_NULL, GPIO_MUX_NULL
};

//AT32F437xx
#define devconf_intr_a0 \
		"SCFG pa0", \
		{CRM_SCFG_PERIPH_CLOCK, SCFG_PORT_SOURCE_GPIOA, SCFG_PINS_SOURCE0}, \
		&pe_a0, /*extend (essential)*/ \
		&gp_a0 /*NULL*/
#define devconf_intr_c7 \
		"SCFG pc7", \
		{CRM_SCFG_PERIPH_CLOCK, SCFG_PORT_SOURCE_GPIOC, SCFG_PINS_SOURCE7}, \
		&pe_c7, /*extend (essential)*/ \
		&gp_c7

const struct modscfg_st devconf_at437_intr_a0 = {
	devconf_intr_a0,
};
const struct modscfg_st devconf_at437_intr_c7 = {
	devconf_intr_c7,
};

//-
#elif defined (AT32F403Axx) || defined (AT32F403xx) || defined (AT32F407xx) || \
	  defined (AT32F413xx)

//[CRM_SCFG_PERIPH_CLOCK] //essential
struct extscfg_st pe_c7 = {
	"enable, extline pc7",
	{ CRM_GPIOC_PERIPH_CLOCK, EXINT_LINE_7, EXINT9_5_IRQn, NVIC_PRIORITY_GROUP_0}, // correspond to and with PC7
};

//[CRM_SCFG_PERIPH_CLOCK] //essential
struct extscfg_st pe_c6 = {
	"enable, extline pc6",
	{ CRM_GPIOC_PERIPH_CLOCK, EXINT_LINE_6, EXINT9_5_IRQn, NVIC_PRIORITY_GROUP_0}, // or NVIC_PRIORITY_GROUP_4, correspond to and with PC7
};

//AT32F403xxx
#define devconf_intr_c6 \
	"GPIO pc6", \
	{CRM_IOMUX_PERIPH_CLOCK, GPIO_PORT_SOURCE_GPIOC, GPIO_PINS_SOURCE6}, \
	&pe_c6, /*essential*/ \
	NULL	// NULL for 403a, &gp for 437.

#define devconf_intr_c7 \
	"GPIO pc7", \
	{CRM_IOMUX_PERIPH_CLOCK, GPIO_PORT_SOURCE_GPIOC, GPIO_PINS_SOURCE7}, \
	&pe_c7, /*essential*/ \
	NULL	// &gp  // for 437.


const struct modscfg_st devconf_at403a_intr_c6 = {
	devconf_intr_c6,
};
const struct modscfg_st devconf_at403a_intr_c7 = {
	devconf_intr_c7,
};

#else
	#error "not defined intrconf_PT"
#endif

//#define exint_data()				(1)
//#define exint_data()				((const struct modscfg_st **)intr_packPT)

//static void config_exint(gpio_pull_type gppull, exint_polarity_config_type polarity)
//{
//  if (intr_gpio_mptr()) {
//	  printf("................................ dm9051 gpio_pin_config(for interr)\r\n");
//	  printf("gpio_pin_config: INTR-gpio\r\n");
//	  gpio_pin_config(intr_gpio_ptr(), gppull);
//  }

//  exint_mconfig(polarity);
//}
