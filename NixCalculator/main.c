/*
 * NixCalculator.c
 *
 * Created: 2022-08-29 3:13:11 PM
 * Author : Jake Alt
 */ 


#include "sam.h"
#include "pins.h"
#include "gpio.h"
#include "SSDisplay.h"
#include "keymap.h"
#include "keypad.h"
#include "buzzer.h"
#include "indicatorlights.h"
#include "NXDisplay.h"
#include "NeoPixel.h"
#include "clocks.h"
#include "colors.h"
#include "Commands/command.h"
#include "types.h"
#include <stdint.h>

#include <stdbool.h>

uint16_t tone_good_freq = 1400;
uint16_t tone_good_duration = 20;
uint16_t tone_bad_freq = 700;
uint16_t tone_bad_duration = 30;


sys_state_t sys_state = {
	.settings = {
		.grad_enabled = 1
	},
	.sys = {
		.angle_units = au_deg,
		.sci_notation = sn_norm,
		.entry_mode = em_rpn,
		.modifiers = {
			.shift = mod_inactive,
			.hyp = mod_inactive
		}
	},
	.ui = {
		.sci_active = sci_mant,
		.cpx_view = cv_re,
		.view_is_real = 1,
		.menu_active = 0
	}
};

calc_state_t calc_state = {
	0
};

static uint32_t systick_timer_ms = 0;

struct {
	SSDisplay	ssd;
	keypad		keypad;
} io_devices;

NXConfig nxconfig = {
	.dp_space = 1,
	.dp_template = NX_DPR
};

void SysTick_Handler(void) {
	static unsigned int tickCountSci = 0, tickCountKeys = 0;
	tickCountSci++; tickCountKeys++;
	if (tickCountSci > io_devices.ssd.update_period_ticks) {
		tickCountSci = 0;
		SSD_updateDisp();
	}
	if (tickCountKeys > 10) {
		tickCountKeys = 0;
		keypad_scan_keys();
	}
	buzzer_timer_tick();
	systick_timer_ms++;
}

void mainfsm() {
	enum {uimode_normal, uimode_mainmenu};
	static int state = uimode_normal;
	
	switch (state) {
		case uimode_normal:
		
		break;
		case uimode_mainmenu:
		break;
	}
}

void GpioInit() {
	gpio_set_pin_dir(GPIO(PWR_LED), GPIO_DIR_OUT);
	gpio_set_pin_dir(GPIO(BUSY_LED), GPIO_DIR_OUT);
	gpio_set_pin_drvstr(GPIO(PWR_LED), 1);
	gpio_set_pin_drvstr(GPIO(BUSY_LED), 1);
	gpio_set_pin_dir(GPIO(PWR_CTRL), GPIO_DIR_OUT);
}

void enable_pwr() {
	// TODO: check voltage level
	gpio_set_pin(GPIO(PWR_CTRL), 1);
}

void LvPeripheralsInit() {
	SSD_init(&io_devices.ssd);
	keypad_init(&io_devices.keypad);
	build_keymap();
	
	buzzer_init();
	fplights_init();
}

void HvPeripheralsInit() {
	enable_pwr();
	
	NeoPixel_init();
	NXDisplay_init(&nxconfig);
}


const color_rgb colors[] = {{32, 32, 32}, {128, 128, 128}, {255, 255, 255}, {0, 64, 200}, {0, 220, 0}, {255, 0, 0}, {190, 0, 128}};

void normal_keypad_lighting() {
	for (int i = 0; i < KEYPAD_GRID_SIZE; i++) {
		const kp_key *currentkey = &keypad_map[i];
		if (currentkey->key_type != 0) {
			// TODO: Menu lighting
			// Modifiers always available (TODO: change this)
			int available = (currentkey->key_type & KEY_TYPE_MOD_MASK) ? 1 : 0;
			color_rgb c = color_off;
			if (currentkey->special_color) {
				// If the key has special color rules, apply that instead
				c = currentkey->special_color(&calc_state, &sys_state);
			} else {
				// Lighting depends on modifiers and command availability
				if (sys_state.sys.modifiers.shift && sys_state.sys.modifiers.hyp) {
					if (currentkey->shift_hyp_cmd && currentkey->shift_hyp_cmd->available != NULL) {
						available = currentkey->shift_hyp_cmd->available(&calc_state, &sys_state);
					}
					c = available ? color_shift_hyp : color_shift_hyp_unavail;
				} else if (sys_state.sys.modifiers.shift) {
					if (currentkey->shift_cmd && currentkey->shift_cmd->available != NULL) {
						available = currentkey->shift_cmd->available(&calc_state, &sys_state);
					}
					c = available ? color_shift : color_shift_unavail;
				} else if (sys_state.sys.modifiers.hyp) {
					if (currentkey->hyp_cmd && currentkey->hyp_cmd->available != NULL) {
						available = currentkey->hyp_cmd->available(&calc_state, &sys_state);
					}
					c = available ? color_hyp : color_hyp_unavail;
				} else {
					if (currentkey->base_cmd && currentkey->base_cmd->available != NULL) {
						available = currentkey->base_cmd->available(&calc_state, &sys_state);
					}
					c = available ? color_normal : color_normal_unavail;
				}
			}
			NeoPixel_set_pixel(currentkey->pixel_idx, c.r, c.g, c.b);
		}
	}
	NeoPixel_update();
	fplights_show(&sys_state);
}

void process_modifiers() {
	
}

cmd_generic *process_keypress() {
	uint64_t keys = io_devices.keypad.changed;
	// clear the pending key press
	io_devices.keypad.changed = 0ULL;
	
	for (int i = 0; i < KEYPAD_GRID_SIZE; i++) {
		// TODO: special rules?
		if (keys & (1ULL << i)) {
			const kp_key *currentkey = &keypad_map[i];
			if (currentkey->key_type != 0) {
				cmd_generic *cmd = NULL;
				uint8_t modifier = (currentkey->key_type & KEY_TYPE_MOD_MASK);
				if (modifier) {
					// Set modifiers if button pressed. If modifier is already active, let it be changed to 'held'
					if (modifier == KEY_TYPE_SHIFT && !sys_state.sys.modifiers.shift) {
						sys_state.sys.modifiers.shift = mod_active;
						BUZZER_TONE_GOOD();
						return NULL; // No command
					}
					if (modifier == KEY_TYPE_HYP && !sys_state.sys.modifiers.hyp) {
						sys_state.sys.modifiers.hyp = mod_active;
						BUZZER_TONE_GOOD();
						return NULL; // No command
					}
				}
				
				// find the command based on the modifiers
				if (sys_state.sys.modifiers.shift && sys_state.sys.modifiers.hyp) {
					cmd = currentkey->shift_hyp_cmd;
				} else if (sys_state.sys.modifiers.shift) {
					cmd = currentkey->shift_cmd;
				} else if (sys_state.sys.modifiers.hyp) {
					cmd = currentkey->hyp_cmd;
				} else if (currentkey->base_cmd) {
					cmd = currentkey->base_cmd;
				}
				// Set any modifiers to 'held'
				if (sys_state.sys.modifiers.shift == mod_active) {
					sys_state.sys.modifiers.shift = mod_held;
				}
				if (sys_state.sys.modifiers.hyp == mod_active) {
					sys_state.sys.modifiers.hyp = mod_held;
				}
				return cmd;
			}
		}
	}
	return NULL;
}

void exec_cmd(const cmd_generic *cmd) {
	if (cmd->available && cmd->available(&calc_state, &sys_state)) {
		BUZZER_TONE_GOOD();
		// check command type
		if ((cmd->cmd_type & CMD_TYPE_MASK) == CMD_TYPE_OP) {
			cmd_op *op = (cmd_op *)cmd;
			op->op_func(&calc_state, &sys_state);
		} else if ((cmd->cmd_type & CMD_TYPE_MASK) == CMD_TYPE_MENU) {
			// a
		} else if ((cmd->cmd_type & CMD_TYPE_MASK) == CMD_TYPE_NUM) {
			cmd_numeric *cmd_num = (cmd_numeric *)cmd;
			char buf[10] = " -0.00000";
			buf[2] = '0'+cmd_num->number;
			NXDisplay_dispStr(buf);
		}
	} else {
		BUZZER_TONE_BAD();
		// TODO: unavailable command pressed
	}
}

int main(void) {
    GpioInit();
	gpio_set_pin(GPIO(PWR_LED), 1);
	gpio_set_pin(GPIO(BUSY_LED), 1);
	ClocksInit();

	LvPeripheralsInit();
	HvPeripheralsInit();
	//enable_pwr();
	//NeoPixel_init();

	SysTick_Config(4800ul);
	gpio_set_pin(GPIO(BUSY_LED), 0);

	while (1) {
		// Clear modifier keys if needed
		if (sys_state.sys.modifiers.shift == mod_held && !(io_devices.keypad.rawKeys & KEY_MOD_SHIFT_IDX)) {
			 sys_state.sys.modifiers.shift = mod_inactive;
		}
		if (sys_state.sys.modifiers.hyp == mod_held && !(io_devices.keypad.rawKeys & KEY_MOD_HYP_IDX)) {
			 sys_state.sys.modifiers.hyp = mod_inactive;
		}
		// Process a key press and run the command
		cmd_generic *pressed_cmd = process_keypress();
		if (pressed_cmd) {
			gpio_set_pin(GPIO(BUSY_LED), 1);
			exec_cmd(pressed_cmd);
			gpio_set_pin(GPIO(BUSY_LED), 0);
		}
		
		normal_keypad_lighting();
		
	}
}
