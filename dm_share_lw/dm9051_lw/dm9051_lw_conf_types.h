#if HELLO_DRIVER_INTERNAL
/* Sanity3.
 */
#ifndef AT32F437xx
/**
  * @purpose make sure to have 'gpio_mux_sel_type'
  * @brief  gpio muxing function selection type (ARTRRY start to support found in AT32F437)
  * @brief  define to declare while if system header files not defined.
  */
typedef enum
{
  GPIO_MUX_0                             	= 0x00, /*!< gpio muxing function selection 0 */
  GPIO_MUX_1                             	= 0x01, /*!< gpio muxing function selection 1 */
  GPIO_MUX_2                             	= 0x02, /*!< gpio muxing function selection 2 */
  GPIO_MUX_3                             	= 0x03, /*!< gpio muxing function selection 3 */
  GPIO_MUX_4                             	= 0x04, /*!< gpio muxing function selection 4 */
  GPIO_MUX_5                             	= 0x05, /*!< gpio muxing function selection 5 */
  GPIO_MUX_6                             	= 0x06, /*!< gpio muxing function selection 6 */
  GPIO_MUX_7                             	= 0x07, /*!< gpio muxing function selection 7 */
  GPIO_MUX_8                             	= 0x08, /*!< gpio muxing function selection 8 */
  GPIO_MUX_9                             	= 0x09, /*!< gpio muxing function selection 9 */
  GPIO_MUX_10                            	= 0x0A, /*!< gpio muxing function selection 10 */
  GPIO_MUX_11                            	= 0x0B, /*!< gpio muxing function selection 11 */
  GPIO_MUX_12                            	= 0x0C, /*!< gpio muxing function selection 12 */
  GPIO_MUX_13                            	= 0x0D, /*!< gpio muxing function selection 13 */
  GPIO_MUX_14                            	= 0x0E, /*!< gpio muxing function selection 14 */
  GPIO_MUX_15                            	= 0x0F  /*!< gpio muxing function selection 15 */
} gpio_mux_sel_type;
#endif

//typedef enum {
//} iomux_t;
#define  IO_MUX_NULL                    	((uint16_t)0x0000) /*!< subordinate  */
#define  IO_MUX_PINREMAP                 	((uint16_t)0x0001) /*!< subordinate mode a (such as f413)*/
#define  IO_MUX_GPIOMUX                 	((uint16_t)0x0002) /*!< subordinate mode b (such as f437) */
#define  IO_CRM_CLOCK                 		((uint16_t)0x0100) /*!< subordinate mode c (such as f413 clock) */

typedef struct spihead_sel_st {
  char *spi_name;
  spi_type *spi_num;        		//= SPIPORT;
  crm_periph_clock_type spi_crm_clk;	//= SPI_CRM_CLK;
  uint16_t iomux;
} spihead_t;

typedef struct gpio_sel_st {
	gpio_type *gpport;        		//= PORT;
	uint16_t pin;           		//= PIN;
	crm_periph_clock_type gpio_crm_clk;  //= CRM_CLK;	
	gpio_mode_type gpio_mode;		//= type
	gpio_pins_source_type pinsrc;
	gpio_mux_sel_type muxsel;
} gpio_t;

typedef struct {
	char *info;
	spihead_t spidef;
	char *cpu_spi_info;	//cpu name is for the purpose to lead the pins, for easy recogition!
	gpio_t wire_sck;
	gpio_t wire_mi;
	gpio_t wire_mo;
	char *cpu_cs_info;
	gpio_t wire_cs;
} spi_dev_t;

typedef struct extint_init_st {
	crm_periph_clock_type scfg_clk;
#if !defined(AT32F437xx)
	gpio_port_source_type scfg_port_src;
	gpio_pins_source_type scfg_pin_src;
#elif defined(AT32F437xx)
	scfg_port_source_type scfg_port_src; //SCFG_PORT_SOURCE_X
	scfg_pins_source_type scfg_pin_src; //SCFG_PINS_SOURCEX
#endif
} extint_init_t;

typedef struct extline_st {
	crm_periph_clock_type intr_crm_clk; //CRM_GPIOC_PERIPH_CLOCK,
	uint32_t extline; //= LINE
	IRQn_Type irqn; //= EXINTn_m
} extline_t;

struct extscfg_st { //struct linescfg_st
	const char *irq_enable_inf;	
	extline_t extline;
};

typedef struct gp_set_st {
	const char *gp_info;	
	const gpio_t gp;
} gp_set_t;

struct modscfg_st {
	const char *scfg_inf;	
	struct extint_init_st scfg_init; //extint_init_t
	struct extscfg_st *extend; //struct linescfg_st *extend; //essential
	struct gp_set_st *option; //gp_set_t
};

// -

typedef struct {
#define DM_TYPE		1
#include "dm_types.h"
} optsex_t;

/* Sanity2.
 */
#ifndef AT32F437xx
//#define scfg_exint_system_ready			0
//#define gpio_mux_sel_type_system_declaration	0
#else
//#define scfg_exint_system_ready			1
//#define gpio_mux_sel_type_system_declaration	1
#endif

#define GPIO_PINSRC_NULL (gpio_pins_source_type) 0
#define GPIO_MUX_NULL    (gpio_mux_sel_type)	 0

#endif //HELLO_DRIVER_INTERNAL
