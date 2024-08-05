/* pattern-fill */
#define	MUX_NULL()	\
	GPIO_MODE_MUX, GPIO_PINSRC_NULL, GPIO_MUX_NULL

//#define OUTPUT_DATA()	\
//	GPIO_MODE_OUTPUT,	GPIO_PINSRC_NULL,	GPIO_MUX_NULL

/* used in struct data declaration */
#define SPI_PINSTD(spiname,spinum,crmclk,iom)				{spiname, spinum, crmclk, iom}
#define GPIO_PINMUX(gpport,pin,crmclk, ptrmux)				{gpport,pin,crmclk, ptrmux} //, MUX_DATA(S, M)
#define GPIO_PINOUT(gpport,pin,crmclk, ptrmux)				{gpport,pin,crmclk, ptrmux} //, OUTPUT_DATA()
#define GPIO_PINNORM(gpport,pin,crmclk, ptrmux)				{gpport,pin,crmclk, ptrmux} //used (AT32F413/415)
#define GPIO_PININ(gpport,pin,crmclk, ptrmux)				{gpport,pin,crmclk, ptrmux} //used (AT32F413/415)

#define MUX_DATA(src,muxto)	\
	GPIO_MODE_MUX, src, muxto,

#define INP_DATA()	\
	GPIO_MODE_INPUT,
#define OUTP_DATA()	\
	GPIO_MODE_OUTPUT,

//#define IS_GPIO_MUX(gpiot)								(gpiot->gpio_mode == GPIO_MODE_MUX)
//#define VALID_MUX_SRCSEL(gpiot)							((gpiot->pinsrc != GPIO_PINSRC_NULL) && (gpiot->muxsel != GPIO_MUX_NULL))

const gpio_mux_t src1_mux6 = { MUX_DATA(GPIO_PINS_SOURCE1, GPIO_MUX_6)};
const gpio_mux_t src2_mux5 = { MUX_DATA(GPIO_PINS_SOURCE2, GPIO_MUX_5)};
const gpio_mux_t src4_mux6 = { MUX_DATA(GPIO_PINS_SOURCE4, GPIO_MUX_6)};
const gpio_mux_t src5_mux5 = { MUX_DATA(GPIO_PINS_SOURCE5, GPIO_MUX_5)};
const gpio_mux_t src6_mux5 = { MUX_DATA(GPIO_PINS_SOURCE6, GPIO_MUX_5)};
const gpio_mux_t src7_mux5 = { MUX_DATA(GPIO_PINS_SOURCE7, GPIO_MUX_5)};

const gpio_mux_t mode_input = { INP_DATA()};
const gpio_mux_t mode_output = { OUTP_DATA()};

const gpio_mux_t mode_null = { GPIO_MODE_NULL,};

const spi_dev_t devconf[BOARD_SPI_COUNT] = {
	#if defined(AT32F437xx) /* && defined(NONE_DEFINED_LABLE) */
		//AT32F437xx
		#define devconf_at437_spi2(info, spi_setting_name, cs_setting_name, intrcfg) \
			{ \
				info, \
				SPI_PINSTD("SPI2", SPI2, CRM_SPI2_PERIPH_CLOCK, IO_MUX_NULL), \
				spi_setting_name, \
				GPIO_PINMUX(GPIOD, GPIO_PINS_1, CRM_GPIOD_PERIPH_CLOCK, &src1_mux6),  /* //SCK */ \
				GPIO_PINMUX(GPIOC, GPIO_PINS_2, CRM_GPIOC_PERIPH_CLOCK, &src2_mux5),	/* //MISO */ \
				GPIO_PINMUX(GPIOD, GPIO_PINS_4, CRM_GPIOD_PERIPH_CLOCK, &src4_mux6),	/* //MOSI */ \
				cs_setting_name, \
				GPIO_PINOUT(GPIOD, GPIO_PINS_0, CRM_GPIOD_PERIPH_CLOCK, &mode_output), /* //(PD0) */ \
				intrcfg, \
			}
		#define devconf_at437_spi4(info, spi_setting_name, cs_setting_name, intrcfg) \
			{ \
				info, \
				SPI_PINSTD("SPI4", SPI4, CRM_SPI4_PERIPH_CLOCK, IO_MUX_NULL), \
				spi_setting_name, \
				GPIO_PINMUX(GPIOE, GPIO_PINS_2, CRM_GPIOE_PERIPH_CLOCK, &src2_mux5), /* //SCK */ \
				GPIO_PINMUX(GPIOE, GPIO_PINS_5, CRM_GPIOE_PERIPH_CLOCK, &src5_mux5),	/* //MISO */ \
				GPIO_PINMUX(GPIOE, GPIO_PINS_6, CRM_GPIOE_PERIPH_CLOCK, &src6_mux5),	/* //MOSI */ \
				cs_setting_name, \
				GPIO_PINOUT(GPIOE, GPIO_PINS_4, CRM_GPIOE_PERIPH_CLOCK, &mode_output), /* //(PE4) */ \
				intrcfg, \
			}
		#define devconf_at437_spi1(info, spi_setting_name, cs_setting_name, intrcfg) \
			{ \
				info, \
				SPI_PINSTD("SPI1", SPI1, CRM_SPI1_PERIPH_CLOCK, IO_MUX_NULL), \
				spi_setting_name, \
				GPIO_PINMUX(GPIOA, GPIO_PINS_5, CRM_GPIOA_PERIPH_CLOCK, &src5_mux5), /* //SCK */ \
				GPIO_PINMUX(GPIOA, GPIO_PINS_6, CRM_GPIOA_PERIPH_CLOCK, &src6_mux5),	/* //MISO */ \
				GPIO_PINMUX(GPIOA, GPIO_PINS_7, CRM_GPIOA_PERIPH_CLOCK, &src7_mux5),	/* //MOSI */ \
				cs_setting_name, \
				GPIO_PINOUT(GPIOA, GPIO_PINS_15, CRM_GPIOA_PERIPH_CLOCK, &mode_output), /* //(PA15) */ \
				intrcfg, \
			}
		//AT32F437xx

#ifdef DRV_INTR_MODE
		devconf_at437_spi1("AT32F437", "sck/mi/mo/ pa5/pa6/pa7", "cs/ pa15", &devconf_at437_intr_c7), //DM9051A BENCH BOARD V1.0
#else
		devconf_at437_spi1("AT32F437", "sck/mi/mo/ pa5/pa6/pa7", "cs/ pa15", NULL), //Note: NULL got no interrupt, and could faii crash! Need debug.
																					//DM9051A BENCH BOARD V1.0
#endif

		devconf_at437_spi2("AT32F437", "sck/mi/mo/ pd1/pc2/pd4", "cs/ pd0", &devconf_at437_intr_a0),
		devconf_at437_spi4("AT32F437", "sck/mi/mo/ pe2/pe5/pe6", "cs/ pe4", &devconf_at437_intr_c7),

		devconf_at437_spi2("AT32F437", "sck/mi/mo/ pd1/pc2/pd4", "cs/ pd0", &devconf_at437_intr_a0),
		//devconf_at437_spi2("AT32F437", "sck/mi/mo/ pd1/pc2/pd4", "cs/ pd0", &devconf_at437_intr_a0),

//		devconf_at437_spi2("AT32F437", "sck/mi/mo/ pd1/pc2/pd4", "cs/ pd0", TBD),
//		devconf_at437_spi4("AT32F437", "sck/mi/mo/ pe2/pe5/pe6", "cs/ pe4", TBD),
//		devconf_at437_spi2("AT32F437", "sck/mi/mo/ pd1/pc2/pd4", "cs/ pd0", TBD),
//		devconf_at437_spi2("AT32F437", "sck/mi/mo/ pd1/pc2/pd4", "cs/ pd0", TBD),
//		devconf_at437_spi1("AT32F437", "sck/mi/mo/ pa5/pa6/pa7", "cs/ pa15", TBD),
	#elif defined (AT32F413xx) || defined (AT32F415xx)
		//(AT32F413/415)
		#define devconf_at413_spi2_0(info, spi_setting_name, cs_setting_name) \
			{ \
				info, \
				SPI_PINSTD("SPI2", SPI2, CRM_SPI2_PERIPH_CLOCK, IO_MUX_NULL), \
				spi_setting_name, \
				GPIO_PINNORM(GPIOB, GPIO_PINS_13, CRM_GPIOB_PERIPH_CLOCK, 	&mode_null), /* //SCK */ \
				GPIO_PINNORM(GPIOB, GPIO_PINS_14, CRM_GPIOB_PERIPH_CLOCK, 	&mode_null), /* //MISO */ \
				GPIO_PINNORM(GPIOB, GPIO_PINS_15, CRM_GPIOB_PERIPH_CLOCK, 	&mode_null), /* //MOSI */ \
				cs_setting_name, \
				GPIO_PINOUT(GPIOB, GPIO_PINS_12, CRM_GPIOB_PERIPH_CLOCK,	&mode_output), /* //(PB12) */ \
				/* GPIO_PINOUT(GPIOA, GPIO_PINS_4, CRM_GPIOA_PERIPH_CLOCK), (PB12) */ \
			}
		#define devconf_at413_spi1_0(info, spi_setting_name, cs_setting_name, gp_port, gp_pin, gp_crm_clk) \
			{ \
				info, \
				SPI_PINSTD("SPI1", SPI1, CRM_SPI1_PERIPH_CLOCK, IO_MUX_NULL), \
				spi_setting_name, \
				GPIO_PINNORM(GPIOB,		GPIO_PINS_3, 	CRM_GPIOB_PERIPH_CLOCK, &mode_null), /* //SCK */ \
				GPIO_PININ(GPIOB,		GPIO_PINS_4, 	CRM_GPIOB_PERIPH_CLOCK, &mode_input), /* //MISO */ \
				GPIO_PINNORM(GPIOB,		GPIO_PINS_5, 	CRM_GPIOB_PERIPH_CLOCK, &mode_null), /* //MOSI */ \
				cs_setting_name, \
				GPIO_PINOUT(gp_port,	gp_pin, 		gp_crm_clk, &mode_output), /* //(PA4) */ \
				/* GPIO_PINOUT(GPIOB,	GPIO_PINS_12,	CRM_GPIOB_PERIPH_CLOCK), (PB12) */ \
				/* GPIO_PINOUT(GPIOA,	GPIO_PINS_4,	CRM_GPIOA_PERIPH_CLOCK), (PA4) */ \
			}

		//AT32F413/415
		#define devconf_at413_spi2(info, spi_setting_name, cs_setting_name) { \
				info, \
				SPI_PINSTD("SPI2", SPI2, CRM_SPI2_PERIPH_CLOCK, IO_MUX_NULL), \
				spi_setting_name, \
				GPIO_PINNORM(GPIOB,		GPIO_PINS_13,	CRM_GPIOB_PERIPH_CLOCK, &mode_null),  /* //SCK */ \
				GPIO_PINNORM(GPIOB,		GPIO_PINS_14,	CRM_GPIOB_PERIPH_CLOCK, &mode_null),	/* //MISO */ \
				GPIO_PINNORM(GPIOB,		GPIO_PINS_15,	CRM_GPIOB_PERIPH_CLOCK, &mode_null),	/* //MOSI */ \
				cs_setting_name, \
				GPIO_PINOUT(GPIOB,	GPIO_PINS_12,		CRM_GPIOB_PERIPH_CLOCK, &mode_output), /* //(PB12) */ \
			}
		#define devconf_at413_spi1a(info, spi_setting_name, cs_setting_name, gp_port, gp_pin, gp_crm_clk, iom) { \
				info, \
				SPI_PINSTD("SPI1", SPI1, CRM_SPI1_PERIPH_CLOCK, iom), \
				spi_setting_name, \
				GPIO_PINNORM(GPIOA,		GPIO_PINS_5,	CRM_GPIOA_PERIPH_CLOCK, &mode_null),  /* //SCK */ \
				GPIO_PININ(GPIOA,		GPIO_PINS_6,	CRM_GPIOA_PERIPH_CLOCK, &mode_input), /* //MISO */ \
				GPIO_PINNORM(GPIOA,		GPIO_PINS_7,	CRM_GPIOA_PERIPH_CLOCK, &mode_null), /* //MOSI */ \
				cs_setting_name, \
				GPIO_PINOUT(gp_port,	gp_pin,			gp_crm_clk,				&mode_output), /* //(PA4) */ \
			}
		#define devconf_at413_spi1b(info, spi_setting_name, cs_setting_name, gp_port, gp_pin, gp_crm_clk, iom) { \
				info, \
				SPI_PINSTD("SPI1", SPI1, CRM_SPI1_PERIPH_CLOCK, iom), \
				spi_setting_name, \
				GPIO_PINNORM(GPIOB,		GPIO_PINS_3,	CRM_GPIOB_PERIPH_CLOCK, &mode_null),  /* //SCK */ \
				GPIO_PININ(GPIOB,		GPIO_PINS_4,	CRM_GPIOB_PERIPH_CLOCK, &mode_input), /* //MISO */ \
				GPIO_PINNORM(GPIOB,		GPIO_PINS_5,	CRM_GPIOB_PERIPH_CLOCK, &mode_null), /* //MOSI */ \
				cs_setting_name, \
				GPIO_PINOUT(gp_port,	gp_pin,			gp_crm_clk,				&mode_output), /* //(PA4) */ \
			}
		//AT32F4xx

#ifdef DRV_INTR_MODE
#else
		devconf_at413_spi2("AT32F413 ETHERNET SPI2", "sck/mi/mo/ pb13/pb14/pb15", "cs/ pb12"),
#endif
		devconf_at413_spi1a("AT32F413 ETHERNET SPI1", "sck/mi/mo/ pa5/pa6/pa7", "cs/ pa15",
			GPIOA, GPIO_PINS_15, CRM_GPIOA_PERIPH_CLOCK, IO_CRM_CLOCK),
		devconf_at413_spi1b("AT32F413 ETHERNET SPI1", "sck/mi/mo/ pb3/pb4/pb5", "cs/ pa15",
			GPIOA, GPIO_PINS_15, CRM_GPIOA_PERIPH_CLOCK, IO_CRM_CLOCK | IO_MUX_PINREMAP), /* NEW-ADDED */

		devconf_at413_spi1a("AT32F413 ETHERNET SPI1", "sck/mi/mo/ pa5/pa6/pa7", "cs/ pa4",
			GPIOA, GPIO_PINS_4, CRM_GPIOA_PERIPH_CLOCK, IO_MUX_NULL),

	#elif defined (AT32F403Axx) || defined (AT32F403xx) || defined (AT32F407xx)
		#define devconf_at403a_spi2(info, spi_setting_name, cs_setting_name) \
			{ \
				info, \
				{"SPI2", SPI2, CRM_SPI2_PERIPH_CLOCK, IO_MUX_NULL}, \
				spi_setting_name, \
				{GPIOB, GPIO_PINS_13, CRM_GPIOB_PERIPH_CLOCK, &mode_null },  /* //ISCK */ /*GPIO_MODE_MUX, GPIO_PINSRC_NULL, GPIO_MUX_NULL*/ \
				{GPIOB, GPIO_PINS_14, CRM_GPIOB_PERIPH_CLOCK, &mode_null },	/* //IMISO */ /*GPIO_MODE_MUX, GPIO_PINSRC_NULL, GPIO_MUX_NULL*/ \
				{GPIOB, GPIO_PINS_15, CRM_GPIOB_PERIPH_CLOCK, &mode_null },	/* //IMOSI */ /*GPIO_MODE_MUX, GPIO_PINSRC_NULL, GPIO_MUX_NULL*/ \
				cs_setting_name, \
				{GPIOB, GPIO_PINS_12, CRM_GPIOB_PERIPH_CLOCK, &mode_output }, /* //(PB12) Test-ISP2 OK */ /* GPIO_MODE_OUTPUT, GPIO_PINSRC_NULL, GPIO_MUX_NULL */ \
				/*{GPIOA, GPIO_PINS_15, CRM_GPIOA_PERIPH_CLOCK, 				GPIO_MODE_OUTPUT, GPIO_PINSRC_NULL, GPIO_MUX_NULL},*/ /* //(PA15) Test-ISP2 OK */ \
			}
		#define devconf_at403a_spi1(info, spi_setting_name, cs_setting_name, gpport, pin, gpio_crm_clk) \
			{ \
				info, \
				{"SPI1", SPI1, CRM_SPI1_PERIPH_CLOCK, IO_MUX_NULL}, \
				spi_setting_name, \
				{GPIOA,		GPIO_PINS_5, 	CRM_GPIOA_PERIPH_CLOCK, &mode_null },  /* //ISCK */ \
				{GPIOA,		GPIO_PINS_6, 	CRM_GPIOA_PERIPH_CLOCK, &mode_null }, /* //IMISO */ \
				{GPIOA,		GPIO_PINS_7, 	CRM_GPIOA_PERIPH_CLOCK, &mode_null }, /* //IMOSI */ \
				cs_setting_name, \
				{gpport,	pin, 			gpio_crm_clk, 			&mode_output }, /* //(PA4) Test-ISP2 OK */ /* GPIO_MODE_OUTPUT, GPIO_PINSRC_NULL, GPIO_MUX_NULL */ \
			}
		#define devconf_at403a_spi1a(info, spi_setting_name, cs_setting_name, gpport, pin, gpio_crm_clk, intrcfg) \
			{ \
				info, \
				{"SPI1a", SPI1, CRM_SPI1_PERIPH_CLOCK, IO_CRM_CLOCK}, \
				spi_setting_name, \
				{GPIOA,		GPIO_PINS_5, 	CRM_GPIOA_PERIPH_CLOCK, &mode_null },  /* //ISCK */ \
				{GPIOA,		GPIO_PINS_6, 	CRM_GPIOA_PERIPH_CLOCK, &mode_null }, /* //IMISO */ \
				{GPIOA,		GPIO_PINS_7, 	CRM_GPIOA_PERIPH_CLOCK, &mode_null }, /* //IMOSI */ \
				cs_setting_name, \
				{gpport,	pin, 			gpio_crm_clk, 			&mode_output }, /* //(PA4) Test-ISP2 OK */ /* GPIO_MODE_OUTPUT, GPIO_PINSRC_NULL, GPIO_MUX_NULL */ \
				intrcfg, \
			}
		//AT32F4xx

#ifdef DRV_INTR_MODE
		devconf_at403a_spi1a("AT32F403A ETHERNET SPI1a", "sck/mi/mo/ pa5/pa6/pa7", "cs/ pa15", GPIOA, GPIO_PINS_15, CRM_GPIOA_PERIPH_CLOCK, &devconf_at403a_intr_c7),
#else
#endif
		devconf_at403a_spi1("AT32F403A ETHERNET SPI1", "sck/mi/mo/ pa5/pa6/pa7", "cs/ pa4", GPIOA, GPIO_PINS_4, CRM_GPIOA_PERIPH_CLOCK),
		devconf_at403a_spi2("AT32F403A ETHERNET SPI2", "sck/mi/mo/ pb13/pb14/pb15", "cs/ pb12"),
		/*!< pa15 must jtag-dp disabled and sw-dp enabled */
		// devconf_at403a_spi1("AT32F403A ETHERNET SPI1", "sck/mi/mo/ pa5/pa6/pa7", "cs/ pa15", GPIOA, GPIO_PINS_15, CRM_GPIOA_PERIPH_CLOCK), /*GPIOB, GPIO_PINS_12, CRM_GPIOB_PERIPH_CLOCK*/
	#else
		#error "not defined board"
	#endif
};

//dm9051_lw_cspi.h
#define info_conf_name()			FIELD_SPIDEV(info)
#define cpu_spi_conf_name()			FIELD_SPIDEV(cpu_spi_info)
#define cpu_cs_conf_name()			FIELD_SPIDEV(cpu_cs_info)
#define spihead()					FIELD_SPIDEV(spidef)

char *mstep_spi_conf_name(void)
{
	return spi_conf_name();
}

char *mstep_conf_info(void) {
	return info_conf_name();
}

char *mstep_conf_cpu_spi_ethernet(void) {
	return cpu_spi_conf_name();
}

char *mstep_conf_cpu_cs_ethernet(void) {
	return cpu_cs_conf_name();
}
