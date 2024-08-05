//
// dm9051_lw_crst_data.h
// - rst configuration data
//

const gp_set_t *option_rst_common = NULL;

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

//.const gp_set_t *gen_gpio = &gp_a08; //DEDICATED! ONLY.
//.const gp_set_t *gen_gpio = &gp_b05; //DEDICATED! ONLY.

/* [intern] */
//static 
int rst_pin_exister(void) {
  return rst_gpio_exister();
}

//static 
int rst_pin_mexist(void) {
  if (rst_pin_exister()) {
    printf(": %s :                 rst-pin/ %s\r\n", "config", rst_gpio_info());
    return 1;
  }
  return 0;
}

/* Such as create/alloc/ and also check. */
//static 
const gp_set_t *get_cpin_init_dataptr(int cpin)
{
	switch (cpin) {
		case CPIN_PA08:
			return &gp_a08;
		case CPIN_PB05:
			return &gp_b05;
	}
	return NULL;
}

//static 
void gpio_pin_level(const gp_set_t *gpptr, int level) {
  //if (gen_gpio_exister()) {
	if (level == 0)
		gpio_bits_reset(gp_gpio_pt(gpptr)->gpport, gp_gpio_pt(&gp_b05)->pin); //gen_gpio_ptr(), gen_gpio_ptr()
	else
		gpio_bits_set(gp_gpio_pt(gpptr)->gpport, gp_gpio_pt(&gp_b05)->pin); //gen_gpio_ptr(), gen_gpio_ptr()
  //}
}
