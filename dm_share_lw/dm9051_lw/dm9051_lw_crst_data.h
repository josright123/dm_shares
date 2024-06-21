//
// dm9051_lw_crst_data.h
// - rst configuration data
//

// PA9=USART1_TX, PA10=USART1_RX for USART1
gp_set_t gp_a10 = {
  "GPIO pa10",
  {GPIOA, GPIO_PINS_10,  CRM_GPIOA_PERIPH_CLOCK, &mode_output, }, //(PA10) OUT-pin //GPIO_MODE_INPUT, GPIO_PINSRC_NULL, GPIO_MUX_NULL
};

gp_set_t gp_a08 = {
  "GPIO pa08",
  {GPIOA, GPIO_PINS_8,  CRM_GPIOA_PERIPH_CLOCK, &mode_output, }, //(PA8) OUT-pin //GPIO_MODE_INPUT, GPIO_PINSRC_NULL, GPIO_MUX_NULL
};

gp_set_t gp_b05 = {
  "GPIO pb05",
  {GPIOB, GPIO_PINS_5,  CRM_GPIOB_PERIPH_CLOCK, &mode_output, }, //(PB5) OUT-pin //GPIO_MODE_INPUT, GPIO_PINSRC_NULL, GPIO_MUX_NULL
};

gp_set_t ip_b05 = {
  "GPIO pb05",
  {GPIOB, GPIO_PINS_5,  CRM_GPIOB_PERIPH_CLOCK, &mode_input, }, //(PB5) OUT-pin //GPIO_MODE_INPUT, GPIO_PINSRC_NULL, GPIO_MUX_NULL
};

const gp_set_t *option_rst_common = NULL;
//const gp_set_t *gen_gpio = &gp_a08; //DEDICATED! ONLY.
const gp_set_t *gen_gpio = &gp_b05; //DEDICATED! ONLY.
const gp_set_t *get_gpio = &ip_b05; //DEDICATED! ONLY.

//[0] rst_common
#define rst_gpio_data()         (option_rst_common)
#define rst_gpio_exister()      (rst_gpio_data() ? 1 : 0)

#define PTR_RSTGPIO(field)      ((option_rst_common)->field)
#define rst_gpio_ptr()          ((const gpio_t *)(&PTR_RSTGPIO(gp)))
#define rst_gpio_info()          PTR_RSTGPIO(gp_info)

//[1] gen_gpio
#define gen_gpio_data()         (gen_gpio)
#define gen_gpio_exister()      (gen_gpio ? 1 : 0)

#define PTR_GENGPIO(field)      ((gen_gpio)->field)
#define gen_gpio_ptr()          ((const gpio_t *)(&PTR_GENGPIO(gp)))
#define gen_gpio_info()          PTR_GENGPIO(gp_info)

//[2] get_gpio
#define get_gpio_data()         (get_gpio)
#define get_gpio_exister()      (get_gpio ? 1 : 0)

#define PTR_GETGPIO(field)      ((get_gpio)->field)
#define get_gpio_ptr()          ((const gpio_t *)(&PTR_GETGPIO(gp)))
#define get_gpio_info()          PTR_GETGPIO(gp_info)

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
