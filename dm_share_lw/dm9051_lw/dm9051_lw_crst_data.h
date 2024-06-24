//
// dm9051_lw_crst_data.h
// - rst configuration data
//

// PA9=USART1_TX, PA10=USART1_RX for USART1
const gp_set_t gp_a10 = {
  "GPIO pa10",
  {GPIOA, GPIO_PINS_10,  CRM_GPIOA_PERIPH_CLOCK, &mode_output, }, //(PA10) OUT-pin //GPIO_MODE_INPUT, GPIO_PINSRC_NULL, GPIO_MUX_NULL
};

const gp_set_t gp_a08 = {
  "GPIO pa08",
  {GPIOA, GPIO_PINS_8,  CRM_GPIOA_PERIPH_CLOCK, &mode_output, }, //(PA8) OUT-pin //GPIO_MODE_INPUT, GPIO_PINSRC_NULL, GPIO_MUX_NULL
};

const gp_set_t gp_b05 = {
  "GPIO pb05",
  {GPIOB, GPIO_PINS_5,  CRM_GPIOB_PERIPH_CLOCK, &mode_output, }, //(PB5) OUT-pin //GPIO_MODE_INPUT, GPIO_PINSRC_NULL, GPIO_MUX_NULL
};

const gp_set_t gp_b04 = {
  "GPIO pb04",
  {GPIOB, GPIO_PINS_4,  CRM_GPIOB_PERIPH_CLOCK, &mode_input, },
};

const gp_set_t *option_rst_common = NULL;

//.const gp_set_t *gen_gpio = &gp_a08; //DEDICATED! ONLY.
//.const gp_set_t *gen_gpio = &gp_b05; //DEDICATED! ONLY.

//[0] rst_common
#define rst_gpio_data()         (option_rst_common)
#define rst_gpio_exister()      (rst_gpio_data() ? 1 : 0)

#define PTR_RSTGPIO(field)      ((option_rst_common)->field)
#define rst_gpio_ptr()          ((const gpio_t *)(&PTR_RSTGPIO(gp)))
#define rst_gpio_info()          PTR_RSTGPIO(gp_info)

//[1] gen_gpio
//.#define gen_gpio_data()      (gen_gpio)
//.#define gen_gpio_exister()   (gen_gpio ? 1 : 0)

//#define PTR_GENGPIO(field)    ((gen_gpio)->field)

//#define gen_gpio_ptr()        ((const gpio_t *)(&PTR_GENGPIO(gp)))
#define gp_gpio_pt(gpptr)		(&(((const gp_set_t *)gpptr)->gp))
//#define gen_gpio_info()       PTR_GENGPIO(gp_info)
#define gp_info(gpptr)			((const gp_set_t *)gpptr)->gp_info

/* [intern] */
static int rst_pin_exister(void) {
  return rst_gpio_exister();
}

static int rst_pin_mexist(void) {
  if (rst_pin_exister()) {
    printf(": %s :                 rst-pin/ %s\r\n", "config", rst_gpio_info());
    return 1;
  }
  return 0;
}

/* Such as create/alloc/ and also check. */
static const gp_set_t *get_cpin_init_dataptr(int cpin_enum)
{
	switch (cpin_enum) {
		case CPIN_ENUM_PA08:
			return &gp_a08;
		case CPIN_ENUM_PB05:
			return &gp_b05;
		case CPIN_ENUM_PB04:
			return &gp_b04;
	}
	return NULL;
}

static void gpio_pin_add(const gp_set_t *gpptr)
{
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_ON, (fmt, ##__VA_ARGS__))
  //if (gen_gpio_exister()) {
	printf("gpio_pin_config: GEN-gpio %d of %s\r\n", de_pin(gp_gpio_pt(gpptr)), gp_info(gpptr)); //gen_gpio_ptr(), //gen_gpio_info(), gen_gpio_data()->gp_info
	gpio_pin_config(gp_gpio_pt(gpptr), GPIO_PULL_UP); //gen_gpio_ptr()
  //}
#undef printf
#define printf(fmt, ...) DM9051_DEBUGF(DM9051_TRACE_DEBUG_OFF, (fmt, ##__VA_ARGS__))
}

//static void gpio_pin_set_level(const gp_set_t *gpptr, int level) {
//	if (level == 0)
//		gpio_bits_reset(gp_gpio_pt(gpptr)->gpport, gp_gpio_pt(&gp_b05)->pin); //gen_gpio_ptr(), gen_gpio_ptr()
//	else
//		gpio_bits_set(gp_gpio_pt(gpptr)->gpport, gp_gpio_pt(&gp_b05)->pin); //gen_gpio_ptr(), gen_gpio_ptr()
//}
