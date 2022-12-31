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
#include "NeoPixel.h"
#include "clocks.h"
#include "colors.h"
#include "Commands/command.h"
#include "types.h"
#include <stdint.h>

#include <stdbool.h>

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

//struct {
	//bool shift_state;
	//bool hyp_state;
	//
//} system_status;

typedef struct {
	uint16_t duration_ms;
	uint16_t freq_hz;
	uint8_t start;
} tone_req_t;

tone_req_t tone_request = {
	.duration_ms = 10,
	.freq_hz = 1000,
	.start = 0
};

void SysTick_Handler(void) {
	static unsigned int tickCountSci = 0, tickCountKeys = 0;
	static uint16_t tone_timer = 0;
	tickCountSci++; tickCountKeys++;
	if (tickCountSci > io_devices.ssd.update_period_ticks) {
		tickCountSci = 0;
		SSD_updateDisp();
	}
	if (tickCountKeys > 10) {
		tickCountKeys = 0;
		keypad_scan_keys();
		//if (io_devices.keypad.changed) {
			//
			//io_devices.keypad.changed = 0;
		//}
	}
	//if (tone_request.start) {
		//// PB16 = speaker (TC6 WO[0])
		//tone_timer = 0;
		//TC6.COUNT16.
	//}
	//if (tone_timer < tone_request.duration_ms) {
		//
	//}
	systick_timer_ms++;
}

void mainfsm() {
	enum {main_input, main_calc, main_menu};
	static int state = main_input;
	
	switch (state) {
		case main_input:
		if (false) {}
		break;
		case main_calc:
		break;
		case main_menu:
		break;
	}
}
void modifierfsm() {
	//enum {mod_inactive, mod_active, mod_held};
	static enum modifier_state shiftstate = mod_inactive;
	
	switch (shiftstate) {
		case mod_inactive: {
			if (io_devices.keypad.changed & KEY_MOD_SHIFT_IDX) {
				shiftstate = mod_active;
			}
			break;
		}
		case mod_active: {
			if (io_devices.keypad.changed & KEY_MOD_SHIFT_IDX) {
				shiftstate = mod_inactive;
			} else if (io_devices.keypad.changed & ~KEY_MOD_HYP_IDX) {
				shiftstate = mod_held;
			}
			break;
		}
		case mod_held: {
			if (!(io_devices.keypad.rawKeys & KEY_MOD_SHIFT_IDX)) {
				shiftstate = mod_inactive;
			}
			break;
		}
	}
	sys_state.sys.modifiers.shift = (shiftstate == mod_inactive) ? mod_inactive : mod_active;
}

void GpioInit() {
	//gpio_set_pin_dir(PWR_LED_PORT, PWR_LED_PIN, GPIO_DIR_OUT);
	//gpio_set_pin_dir(BUSY_LED_PORT, BUSY_LED_PORT, GPIO_DIR_OUT);
	//gpio_set_pin_drvstr(PWR_LED_PORT, PWR_LED_PIN, 1);
	//gpio_set_pin_drvstr(BUSY_LED_PORT, BUSY_LED_PORT, 1);
	gpio_set_pin_dir(GPIO(PWR_LED), GPIO_DIR_OUT);
	gpio_set_pin_dir(GPIO(BUSY_LED), GPIO_DIR_OUT);
	gpio_set_pin_drvstr(GPIO(PWR_LED), 1);
	gpio_set_pin_drvstr(GPIO(BUSY_LED), 1);
	gpio_set_pin_dir(GPIO(PWR_CTRL), GPIO_DIR_OUT);
		
}

void enablePwr() {
	// TODO: check voltage level
	gpio_set_pin(GPIO(PWR_CTRL), 1);
}

void LvPeripheralsInit() {
	SSD_init(&io_devices.ssd);
	keypad_init(&io_devices.keypad);
	build_keymap();
}

void HvPeripheralsInit() {
	enablePwr();
	
	NeoPixel_init();
	keypad_init(&io_devices.keypad);
	build_keymap();
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
						return NULL; // No command
					}
					if (modifier == KEY_TYPE_HYP && !sys_state.sys.modifiers.hyp) {
						sys_state.sys.modifiers.hyp = mod_active;
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
		// check command type
		if ((cmd->cmd_type & CMD_TYPE_MASK) == CMD_TYPE_OP) {
			cmd_op *op = (cmd_op *)cmd;
			op->op_func(&calc_state, &sys_state);
		} else if ((cmd->cmd_type & CMD_TYPE_MASK) == CMD_TYPE_MENU) {
			// a
		} else if ((cmd->cmd_type & CMD_TYPE_MASK) == CMD_TYPE_NUM) {
			cmd_numeric *cmd_num = (cmd_numeric *)cmd;
			//cmd_num->num_func(NULL);
		}
	} else {
		// TODO: unavailable command pressed
	}
}

int main(void) {
    GpioInit();
	gpio_set_pin(GPIO(PWR_LED), 1);
	gpio_set_pin(GPIO(BUSY_LED), 1);
	ClocksInit();

	LvPeripheralsInit();

	enablePwr();
	NeoPixel_init();

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
		
		
		//for (int i = 0; i < 240000UL; i++) {}
		//for (int j = 0; j < 7; j++) {
			//for (int i = 0; i < NUM_PIXELS; i++) {
				//NeoPixel_set_pixel(i, colors[j].r, colors[j].g, colors[j].b);
				//NeoPixel_update();
				//for (int i = 0; i < 960000UL; i++) {}
			//}
			//
		//}
	}
}
