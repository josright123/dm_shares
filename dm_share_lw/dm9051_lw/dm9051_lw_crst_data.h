//
// dm9051_lw_crst_data.h
// - rst configuration data
//

const gp_set_t *option_rst_common = NULL;

#define rst_gpio_data()				(option_rst_common)
#define rst_gpio_exister()			(rst_gpio_data() ? 1 : 0)

#define rst_gpio_info()				PTR_RSTGPIO(gp_info)
#define rst_gpio_ptr()				((const gpio_t *)(&PTR_RSTGPIO(gp)))

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

