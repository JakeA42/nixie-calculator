/*
 * indicatorlights.c
 *
 * Created: 2023-01-03 8:02:33 PM
 *  Author: Jake Alt
 */ 

#include "indicatorlights.h"
#include "types.h"
#include "pins.h"
#include "gpio.h"

// LSB first shift register
#define L_DEG  (1 << 7)
#define L_RAD  (1 << 6)
#define L_GRAD (1 << 5)

#define L_REAL		(1 << 4)
#define L_CPX_RE	(1 << 3)
#define L_CPX_IM	(1 << 2)
#define L_CPX_R		(1 << 1)
#define L_CPX_THETA (1 << 0)

void fplights_init() {
	gpio_set_pin_dir(GPIO(FPLIGHT_MOSI), GPIO_DIR_OUT);
	gpio_set_pin_dir(GPIO(FPLIGHT_SCK), GPIO_DIR_OUT);
	gpio_set_pin_dir(GPIO(FPLIGHT_LATCH), GPIO_DIR_OUT);
	gpio_set_pin_dir(GPIO(FPLIGHT_OE), GPIO_DIR_OUT);
	gpio_set_pin(GPIO(FPLIGHT_MOSI), 0);
	gpio_set_pin(GPIO(FPLIGHT_SCK), 0);
	gpio_set_pin(GPIO(FPLIGHT_LATCH), 0);
	gpio_set_pin(GPIO(FPLIGHT_OE), 1);
}

void fplights_show(const sys_state_t *sys_state) {
	uint8_t lights = 0;
	switch (sys_state->sys.angle_units) {
		case au_deg:
		lights |= (L_DEG);
		break;
		case au_rad:
		lights |= (L_RAD);
		break;
		case au_grad:
		lights |= (L_GRAD);
		break;
	}
	lights |= sys_state->ui.view_is_real ? L_REAL : 0;
	switch (sys_state->ui.cpx_view) {
		case cv_re:
			lights |= sys_state->ui.view_is_real ? L_REAL : L_CPX_RE;
		break;
		case cv_im:
			lights |= L_CPX_IM;
		break;
		case cv_r:
			lights |= L_CPX_R;
		break;
		case cv_theta:
			lights |= L_CPX_THETA;
		break;
	}
	gpio_set_pin(GPIO(FPLIGHT_OE), 1);

	for (int i = 0; i < 8; i++) {
		gpio_set_pin(GPIO(FPLIGHT_SCK), 0);
		gpio_set_pin(GPIO(FPLIGHT_MOSI), lights & 1);
		asm("nop\nnop\nnop\nnop\nnop");
		gpio_set_pin(GPIO(FPLIGHT_SCK), 1);
		asm("nop\nnop\nnop\nnop\nnop");
		lights >>= 1;
	}
	gpio_set_pin(GPIO(FPLIGHT_LATCH), 0);
	asm("nop\nnop\nnop\nnop\nnop");
	gpio_set_pin(GPIO(FPLIGHT_LATCH), 1);
	asm("nop\nnop\nnop\nnop\nnop");
	gpio_set_pin(GPIO(FPLIGHT_LATCH), 0);
	gpio_set_pin(GPIO(FPLIGHT_OE), 0);

}
