const spi_dev_t devconf[BOARD_SPI_COUNT] = {
	#ifdef AT32F437xx
		//AT32F437xx
		#define devconf_at437_spi2(info, spi_setting_name, cs_setting_name, intrcfg) \
			{ \
				info, \
				SPI_PINSTD("SPI2", SPI2, CRM_SPI2_PERIPH_CLOCK, IO_MUX_NULL), \
				spi_setting_name, \
				GPIO_PINMUX(GPIOD, GPIO_PINS_1, CRM_GPIOD_PERIPH_CLOCK, GPIO_PINS_SOURCE1, GPIO_MUX_6),  /* //SCK */ \
				GPIO_PINMUX(GPIOC, GPIO_PINS_2, CRM_GPIOC_PERIPH_CLOCK, GPIO_PINS_SOURCE2, GPIO_MUX_5),	/* //MISO */ \
				GPIO_PINMUX(GPIOD, GPIO_PINS_4, CRM_GPIOD_PERIPH_CLOCK, GPIO_PINS_SOURCE4, GPIO_MUX_6),	/* //MOSI */ \
				cs_setting_name, \
				GPIO_PINOUT(GPIOD, GPIO_PINS_0, CRM_GPIOD_PERIPH_CLOCK), /* //(PD0) */ \
				intrcfg, \
			}
		#define devconf_at437_spi4(info, spi_setting_name, cs_setting_name, intrcfg) \
			{ \
				info, \
				SPI_PINSTD("SPI4", SPI4, CRM_SPI4_PERIPH_CLOCK, IO_MUX_NULL), \
				spi_setting_name, \
				GPIO_PINMUX(GPIOE, GPIO_PINS_2, CRM_GPIOE_PERIPH_CLOCK, GPIO_PINS_SOURCE2, GPIO_MUX_5), /* //SCK */ \
				GPIO_PINMUX(GPIOE, GPIO_PINS_5, CRM_GPIOE_PERIPH_CLOCK, GPIO_PINS_SOURCE5, GPIO_MUX_5),	/* //MISO */ \
				GPIO_PINMUX(GPIOE, GPIO_PINS_6, CRM_GPIOE_PERIPH_CLOCK, GPIO_PINS_SOURCE6, GPIO_MUX_5),	/* //MOSI */ \
				cs_setting_name, \
				GPIO_PINOUT(GPIOE, GPIO_PINS_4, CRM_GPIOE_PERIPH_CLOCK), /* //(PE4) */ \
				intrcfg, \
			}
		#define devconf_at437_spi1(info, spi_setting_name, cs_setting_name, intrcfg) \
			{ \
				info, \
				SPI_PINSTD("SPI1", SPI1, CRM_SPI1_PERIPH_CLOCK, IO_MUX_NULL), \
				spi_setting_name, \
				GPIO_PINMUX(GPIOA, GPIO_PINS_5, CRM_GPIOA_PERIPH_CLOCK, GPIO_PINS_SOURCE5, GPIO_MUX_5), /* //SCK */ \
				GPIO_PINMUX(GPIOA, GPIO_PINS_6, CRM_GPIOA_PERIPH_CLOCK, GPIO_PINS_SOURCE6, GPIO_MUX_5),	/* //MISO */ \
				GPIO_PINMUX(GPIOA, GPIO_PINS_7, CRM_GPIOA_PERIPH_CLOCK, GPIO_PINS_SOURCE7, GPIO_MUX_5),	/* //MOSI */ \
				cs_setting_name, \
				GPIO_PINOUT(GPIOA, GPIO_PINS_15, CRM_GPIOA_PERIPH_CLOCK), /* //(PA15) */ \
				intrcfg, \
			}
		//AT32F437xx
			
		devconf_at437_spi2("AT32F437", "sck/mi/mo/ pd1/pc2/pd4", "cs/ pd0", &devconf_at437_intr_a0),
		devconf_at437_spi4("AT32F437", "sck/mi/mo/ pe2/pe5/pe6", "cs/ pe4", &devconf_at437_intr_c7),
		
		devconf_at437_spi2("AT32F437", "sck/mi/mo/ pd1/pc2/pd4", "cs/ pd0", &devconf_at437_intr_a0),
		devconf_at437_spi2("AT32F437", "sck/mi/mo/ pd1/pc2/pd4", "cs/ pd0", &devconf_at437_intr_a0),
		devconf_at437_spi1("AT32F437", "sck/mi/mo/ pa5/pa6/pa7", "cs/ pa15", NULL),
//		devconf_at437_spi2("AT32F437", "sck/mi/mo/ pd1/pc2/pd4", "cs/ pd0", TBD),
//		devconf_at437_spi4("AT32F437", "sck/mi/mo/ pe2/pe5/pe6", "cs/ pe4", TBD),
//		devconf_at437_spi2("AT32F437", "sck/mi/mo/ pd1/pc2/pd4", "cs/ pd0", TBD),
//		devconf_at437_spi2("AT32F437", "sck/mi/mo/ pd1/pc2/pd4", "cs/ pd0", TBD),
//		devconf_at437_spi1("AT32F437", "sck/mi/mo/ pa5/pa6/pa7", "cs/ pa15", TBD),
	#elif defined (AT32F413xx) || defined (AT32F415xx)
		//(AT32F413/415)
		#define GPIO_PININ(gpport,pin,crm_clk)		{gpport,pin,crm_clk, GPIO_MODE_INPUT, GPIO_PINSRC_NULL, GPIO_MUX_NULL}
		#define devconf_at413_spi2_0(info, spi_setting_name, cs_setting_name) \
			{ \
				info, \
				SPI_PINSTD("SPI2", SPI2, CRM_SPI2_PERIPH_CLOCK, IO_MUX_NULL), \
				spi_setting_name, \
				GPIO_PINNORM(GPIOB, GPIO_PINS_13, CRM_GPIOB_PERIPH_CLOCK), /* //SCK */ \
				GPIO_PINNORM(GPIOB, GPIO_PINS_14, CRM_GPIOB_PERIPH_CLOCK), /* //MISO */ \
				GPIO_PINNORM(GPIOB, GPIO_PINS_15, CRM_GPIOB_PERIPH_CLOCK), /* //MOSI */ \
				cs_setting_name, \
				GPIO_PINOUT(GPIOB, GPIO_PINS_12, CRM_GPIOB_PERIPH_CLOCK), /* //(PB12) */ \
				/* GPIO_PINOUT(GPIOA, GPIO_PINS_4, CRM_GPIOA_PERIPH_CLOCK), (PB12) */ \
			}
		#define devconf_at413_spi1_0(info, spi_setting_name, cs_setting_name, gp_port, gp_pin, gp_crm_clk) \
			{ \
				info, \
				SPI_PINSTD("SPI1", SPI1, CRM_SPI1_PERIPH_CLOCK, IO_MUX_NULL), \
				spi_setting_name, \
				GPIO_PINNORM(GPIOB,		GPIO_PINS_3, 	CRM_GPIOB_PERIPH_CLOCK), /* //SCK */ \
				GPIO_PININ(GPIOB,		GPIO_PINS_4, 	CRM_GPIOB_PERIPH_CLOCK), /* //MISO */ \
				GPIO_PINNORM(GPIOB,		GPIO_PINS_5, 	CRM_GPIOB_PERIPH_CLOCK), /* //MOSI */ \
				cs_setting_name, \
				GPIO_PINOUT(gp_port,	gp_pin, 		gp_crm_clk), /* //(PA4) */ \
				/* GPIO_PINOUT(GPIOB,	GPIO_PINS_12,	CRM_GPIOB_PERIPH_CLOCK), (PB12) */ \
				/* GPIO_PINOUT(GPIOA,	GPIO_PINS_4,	CRM_GPIOA_PERIPH_CLOCK), (PA4) */ \
			}

		//AT32F413/415
		#define devconf_at413_spi2(info, spi_setting_name, cs_setting_name) { \
				info, \
				SPI_PINSTD("SPI2", SPI2, CRM_SPI2_PERIPH_CLOCK, IO_MUX_NULL), \
				spi_setting_name, \
				GPIO_PINNORM(GPIOB,		GPIO_PINS_13,	CRM_GPIOB_PERIPH_CLOCK),  /* //SCK */ \
				GPIO_PINNORM(GPIOB,		GPIO_PINS_14,	CRM_GPIOB_PERIPH_CLOCK),	/* //MISO */ \
				GPIO_PINNORM(GPIOB,		GPIO_PINS_15,	CRM_GPIOB_PERIPH_CLOCK),	/* //MOSI */ \
				cs_setting_name, \
				GPIO_PINOUT(GPIOB,	GPIO_PINS_12,	CRM_GPIOB_PERIPH_CLOCK), /* //(PB12) */ \
			}
		#define devconf_at413_spi1a(info, spi_setting_name, cs_setting_name, gp_port, gp_pin, gp_crm_clk, iom) { \
				info, \
				SPI_PINSTD("SPI1", SPI1, CRM_SPI1_PERIPH_CLOCK, iom), \
				spi_setting_name, \
				GPIO_PINNORM(GPIOA,		GPIO_PINS_5,	CRM_GPIOA_PERIPH_CLOCK),  /* //SCK */ \
				GPIO_PININ(GPIOA,		GPIO_PINS_6,	CRM_GPIOA_PERIPH_CLOCK), /* //MISO */ \
				GPIO_PINNORM(GPIOA,		GPIO_PINS_7,	CRM_GPIOA_PERIPH_CLOCK), /* //MOSI */ \
				cs_setting_name, \
				GPIO_PINOUT(gp_port,	gp_pin,			gp_crm_clk), /* //(PA4) */ \
			}
		#define devconf_at413_spi1b(info, spi_setting_name, cs_setting_name, gp_port, gp_pin, gp_crm_clk, iom) { \
				info, \
				SPI_PINSTD("SPI1", SPI1, CRM_SPI1_PERIPH_CLOCK, iom), \
				spi_setting_name, \
				GPIO_PINNORM(GPIOB,		GPIO_PINS_3,	CRM_GPIOB_PERIPH_CLOCK),  /* //SCK */ \
				GPIO_PININ(GPIOB,		GPIO_PINS_4,	CRM_GPIOB_PERIPH_CLOCK), /* //MISO */ \
				GPIO_PINNORM(GPIOB,		GPIO_PINS_5,	CRM_GPIOB_PERIPH_CLOCK), /* //MOSI */ \
				cs_setting_name, \
				GPIO_PINOUT(gp_port,	gp_pin,			gp_crm_clk), /* //(PA4) */ \
			}
		//AT32F4xx
		
		devconf_at413_spi2("AT32F413 ETHERNET SPI2", "sck/mi/mo/ pb13/pb14/pb15", "cs/ pb12"),
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
				{"SPI2", SPI2, CRM_SPI2_PERIPH_CLOCK}, \
				spi_setting_name, \
				{GPIOB, GPIO_PINS_13, CRM_GPIOB_PERIPH_CLOCK, GPIO_MODE_MUX,    GPIO_PINSRC_NULL, GPIO_MUX_NULL},  /* //ISCK */ \
				{GPIOB, GPIO_PINS_14, CRM_GPIOB_PERIPH_CLOCK, GPIO_MODE_MUX,	GPIO_PINSRC_NULL, GPIO_MUX_NULL},	/* //IMISO */ \
				{GPIOB, GPIO_PINS_15, CRM_GPIOB_PERIPH_CLOCK, GPIO_MODE_MUX,	GPIO_PINSRC_NULL, GPIO_MUX_NULL},	/* //IMOSI */ \
				cs_setting_name, \
				{GPIOB, GPIO_PINS_12, CRM_GPIOB_PERIPH_CLOCK, GPIO_MODE_OUTPUT, GPIO_PINSRC_NULL, GPIO_MUX_NULL}, /* //(PB12) Test-ISP2 OK */ \
				/*{GPIOA, GPIO_PINS_15, CRM_GPIOA_PERIPH_CLOCK, 				GPIO_MODE_OUTPUT, GPIO_PINSRC_NULL, GPIO_MUX_NULL},*/ /* //(PA15) Test-ISP2 OK */ \
			}
		#define devconf_at403a_spi1(info, spi_setting_name, cs_setting_name, gpport, pin, gpio_crm_clk) \
			{ \
				info, \
				{"SPI1",	SPI1,			CRM_SPI1_PERIPH_CLOCK}, \
				spi_setting_name, \
				{GPIOA,		GPIO_PINS_5, 	CRM_GPIOA_PERIPH_CLOCK, 	GPIO_MODE_MUX, 	GPIO_PINSRC_NULL, GPIO_MUX_NULL},  /* //ISCK */ \
				{GPIOA,		GPIO_PINS_6, 	CRM_GPIOA_PERIPH_CLOCK, 	GPIO_MODE_MUX,	GPIO_PINSRC_NULL, GPIO_MUX_NULL}, /* //IMISO */ \
				{GPIOA,		GPIO_PINS_7, 	CRM_GPIOA_PERIPH_CLOCK, 	GPIO_MODE_MUX,	GPIO_PINSRC_NULL, GPIO_MUX_NULL}, /* //IMOSI */ \
				cs_setting_name, \
				{gpport,	pin, 			gpio_crm_clk, 				GPIO_MODE_OUTPUT, GPIO_PINSRC_NULL, GPIO_MUX_NULL}, /* //(PA4) Test-ISP2 OK */ \
			}
		//AT32F4xx
		
		devconf_at403a_spi1("AT32F403A ETHERNET SPI1", "sck/mi/mo/ pa5/pa6/pa7", "cs/ pa4", GPIOA, GPIO_PINS_4, CRM_GPIOA_PERIPH_CLOCK),
		devconf_at403a_spi2("AT32F403A ETHERNET SPI2", "sck/mi/mo/ pb13/pb14/pb15", "cs/ pb12"),	
		/*!< pa15 must jtag-dp disabled and sw-dp enabled */
		// devconf_at403a_spi1("AT32F403A ETHERNET SPI1", "sck/mi/mo/ pa5/pa6/pa7", "cs/ pa15", /*GPIOB, GPIO_PINS_12, CRM_GPIOB_PERIPH_CLOCK*/ GPIOA, GPIO_PINS_15, CRM_GPIOA_PERIPH_CLOCK),
	#else
		#error "not defined board"
	#endif
};
