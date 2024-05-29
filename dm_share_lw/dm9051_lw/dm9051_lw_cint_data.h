//
// dm9051_lw_cint_data.h
// - intr configuration data
//

/* [intern] */
#define scfg_info()					PTR_EXINTD(scfg_inf)
#define scfg_crm()					PTR_EXINTD(scfg_init.scfg_clk)
#define scfg_port()					PTR_EXINTD(scfg_init.scfg_port_src)
#define scfg_pin()					PTR_EXINTD(scfg_init.scfg_pin_src)

#define exint_enable_info()			PTR_EXINTD(extend->irq_enable_inf)
#define exint_extline()				PTR_EXINTD(extend->extline.extline)
#define exint_crm()					PTR_EXINTD(extend->extline.intr_crm_clk)

//#define exint_exister()			((struct modscfg_st *)intr_packPT)
//#define exint_scfg_ptr()			!exint_data() ? NULL : ((struct modscfg_st *)intr_packPT[pin_code])->extend
//#define intr_gpio_exister()			!exint_data() ? 0 : !(((struct modscfg_st *)intr_packPT[pin_code])->option) ? 0 : 1

//#define exint_data()				(1)
#define exint_scfg_ptr()			FIELD_SPIDEV(intr_cfg)->extend
#define intr_gpio_ptr()				((const gpio_t *)(&PTR_EXINTD(option->gp)))
//#define exint_data()				((const struct modscfg_st **)intr_packPT)
//#define exint_scfg_ptr()			!exint_data() ? NULL : ((const struct modscfg_st **)intr_packPT)[pin_code]->extend
//#define intr_gpio_exister()		!exint_data() ? 0 : !(((const struct modscfg_st **)intr_packPT)[pin_code]->option) ? 0 : 1

//#define intr_gpio_exister()			!(FIELD_SPIDEV(intr_cfg)->option) ? 0 : 1
//static int intr_gpio_exist(void) {
//	return intr_gpio_exister();
//}

int intr_gpio_mptr(void) {
//	return intr_gpio_exist() ? 1 : 0;
	return intr_pointer() ? 1 : 0;
}

//static void config_exint(gpio_pull_type gppull, exint_polarity_config_type polarity)
//{
//  if (intr_gpio_mptr()) {
//	  printf("................................ dm9051 gpio_pin_config(for interr)\r\n");
//	  printf("gpio_pin_config: INTR-gpio\r\n");
//	  gpio_pin_config(intr_gpio_ptr(), gppull);
//  }

//  exint_mconfig(polarity);
//}
