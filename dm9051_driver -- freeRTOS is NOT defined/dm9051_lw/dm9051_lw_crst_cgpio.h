#ifndef __DM9051_LW_CRST_H__
#define __DM9051_LW_CRST_H__

//#include "dm9051_lw_conf_types.h"

extern const gp_set_t *option_rst_common;

void gpio_pin_add(const gp_set_t *gpptr);
void gpio_pin_level(const gp_set_t *gpptr, int level);

const gp_set_t *get_cpin_init_dataptr(int cpin);
int rst_pin_exister(void);
int rst_pin_mexist(void);

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

#endif //__DM9051_LW_CRST_H__
