/*
 * keymap.c
 *
 * Created: 2022-12-21 11:53:03 AM
 *  Author: Jake Alt
 */ 

#include "keymap.h"
#include "colors.h"
#include "types.h"
#include "Commands/commands.h"
/*
{ 
	.key_type = KEY_TYPE_STD,
	.base_cmd = NULL,
	.shift_cmd = NULL,
	.hyp_cmd = NULL,
	.shift_hyp_cmd = NULL
},
*/
//const kp_key keypad_map[KEYPAD_NUM_COLS * KEYPAD_NUM_ROWS] = {0};


color_rgb standard_color(const kp_key * currentkey, const calc_state_t * calc_state, const sys_state_t * sys_state) {
	color_rgb c;
	int available = 0;
	// Modifiers always available (TODO: change this)
	available = (currentkey->key_type & KEY_TYPE_MOD_MASK) ? 1 : 0;

	if (sys_state->mods.shift && sys_state->mods.hyp) {
		if (currentkey->shift_hyp_cmd && currentkey->shift_hyp_cmd->available != NULL) {
			available = currentkey->shift_hyp_cmd->available(calc_state, sys_state);
		}
		c = available ? color_shift_hyp : color_shift_hyp_unavail;
	} else if (sys_state->mods.shift) {
		if (currentkey->shift_cmd && currentkey->shift_cmd->available != NULL) {
			available = currentkey->shift_cmd->available(calc_state, sys_state);
		}
		c = available ? color_shift : color_shift_unavail;
	} else if (sys_state->mods.hyp) {
		if (currentkey->hyp_cmd && currentkey->hyp_cmd->available != NULL) {
			available = currentkey->hyp_cmd->available(calc_state, sys_state);
		}
		c = available ? color_hyp : color_hyp_unavail;
	} else {
		if (currentkey->base_cmd && currentkey->base_cmd->available != NULL) {
			available = currentkey->base_cmd->available(calc_state, sys_state);
		}
		c = available ? color_normal : color_normal_unavail;
	}
	return c;
}

// Determines the lighting of the re/im key based on the current view
static color_rgb re_im_key_color(const kp_key * currentkey, const calc_state_t * calc_state, const sys_state_t * sys_state) {
	switch (sys_state->ui.cpx_view) {
		case cv_re:
		return sys_state->ui.view_is_real ? color_normal : color_cplx_re;
		case cv_im:
		return color_cplx_im;
		case cv_r:
		return color_cplx_r;
		case cv_theta:
		return color_cplx_theta;
	}
	return color_off;
}

static color_rgb drg_key_color(const kp_key * currentkey, const calc_state_t * calc_state, const sys_state_t * sys_state) {
	switch (sys_state->sys.angle_units) {
		case au_deg:
		return color_drg_d;
		case au_rad:
		return color_drg_r;
		case au_grad:
		return color_drg_g;
	}
	return color_off;
}

static color_rgb shift_key_color(const kp_key * currentkey, const calc_state_t * calc_state, const sys_state_t * sys_state) {
	return sys_state->mods.shift ? (sys_state->mods.hyp ? color_shift_hyp : color_shift) : color_normal;
}
static color_rgb hyp_key_color(const kp_key * currentkey, const calc_state_t * calc_state, const sys_state_t * sys_state) {
	return sys_state->mods.hyp ? (sys_state->mods.shift ? color_shift_hyp : color_hyp) : color_normal;
}



void build_keymap() {
	keypad_map[0] = (kp_key){0};
	keypad_map[1] = (kp_key){0};
	keypad_map[2] = (kp_key){ //menu/pwr key
		.key_type = KEY_TYPE_STD | KEY_TYPE_MENU_PWR,
		.base_cmd = NULL, //TODO: menu
		.shift_cmd = NULL, // TODO: power
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 12,
		.special_color = NULL
	};
	keypad_map[3] = (kp_key){ //drg key
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_drg_reint, //TODO: reinterpret drg
		.shift_cmd = NULL, // TODO: convert drg
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 27,
		.special_color = drg_key_color
	};
	keypad_map[4] = (kp_key){ //re/im key
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_re_im,
		.shift_cmd = (cmd_generic*)cmd_rec_pol,
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 28,
		.special_color = re_im_key_color
	};
		
	// COL 1 ////////////////////////////
	keypad_map[5] = (kp_key){0};
	keypad_map[6] = (kp_key){0};
	keypad_map[7] = (kp_key){ // EEX
		.key_type = KEY_TYPE_STD,
		.base_cmd = NULL, // TODO: EEX
		.shift_cmd = NULL, // TODO: sc/en
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 13,
		.special_color = NULL
	};
	keypad_map[8] = (kp_key){ // roll up/down
		.key_type = KEY_TYPE_STD,
		.base_cmd = NULL, // TODO: roll up
		.shift_cmd = NULL, // TODO: roll down
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 26,
		.special_color = NULL
	};
	keypad_map[9] = (kp_key){ // 2nd
		.key_type = KEY_TYPE_SHIFT,
		.base_cmd = NULL,
		.shift_cmd = NULL,
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 29,
		.special_color = shift_key_color
	};
		
	// COL 2 ////////////////////////////
	keypad_map[10] = (kp_key){ // C/AC
		.key_type = KEY_TYPE_STD | KEY_TYPE_CLR,
		.base_cmd = (cmd_generic*)cmd_clear, // TODO: C
		.shift_cmd = (cmd_generic*)cmd_clearall, // TODO: AC
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 0,
		.special_color = NULL // TODO: clear colors?
	};
	keypad_map[11] = (kp_key){ // bksp
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_bksp, // TODO: bksp
		.shift_cmd = NULL,
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 11,
		.special_color = NULL
	};
	keypad_map[12] = (kp_key){ // +/-
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_chs, // TODO: +/-
		.shift_cmd = (cmd_generic*)cmd_abs,
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 14,
		.special_color = NULL
	};
	keypad_map[13] = (kp_key){ // enter
		.key_type = KEY_TYPE_STD,
		.base_cmd = NULL, // TODO: enter
		.shift_cmd = NULL,
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 25,
		.special_color = NULL
	};
	keypad_map[14] = (kp_key){0};
		
	// COL 3 ////////////////////////////
	keypad_map[15] = (kp_key){ // sin
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_sin,
		.shift_cmd = (cmd_generic*)cmd_asin,
		.hyp_cmd = (cmd_generic*)cmd_sinh,
		.shift_hyp_cmd = (cmd_generic*)cmd_asinh,
		.pixel_idx = 1,
		.special_color = NULL
	};
	keypad_map[16] = (kp_key){ // 7
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_7,
		.shift_cmd = NULL,
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 10,
		.special_color = NULL
	};
	keypad_map[17] = (kp_key){ // 4
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_4,
		.shift_cmd = NULL,
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 15,
		.special_color = NULL
	};
	keypad_map[18] = (kp_key){ // 1
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_1,
		.shift_cmd = (cmd_generic*)cmd_i,
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 24,
		.special_color = NULL
	};
	keypad_map[19] = (kp_key){ // 0
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_0,
		.shift_cmd = NULL, // TODO: Mem
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 30,
		.special_color = NULL
	};
		
	// COL 4 ////////////////////////////
	keypad_map[20] = (kp_key){ // cos
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_cos,
		.shift_cmd = (cmd_generic*)cmd_acos,
		.hyp_cmd = (cmd_generic*)cmd_cosh,
		.shift_hyp_cmd = (cmd_generic*)cmd_acosh,
		.pixel_idx = 2,
		.special_color = NULL
	};
	keypad_map[21] = (kp_key){ // 8
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_8,
		.shift_cmd = NULL,
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 9,
		.special_color = NULL
	};
	keypad_map[22] = (kp_key){ // 5
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_5,
		.shift_cmd = NULL,
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 16,
		.special_color = NULL
	};
	keypad_map[23] = (kp_key){ // 2
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_2,
		.shift_cmd = (cmd_generic*)cmd_e,
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 23,
		.special_color = NULL
	};
	keypad_map[24] = (kp_key){0};

	// COL 5 ////////////////////////////
	keypad_map[25] = (kp_key){ // tan
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_tan,
		.shift_cmd = (cmd_generic*)cmd_atan,
		.hyp_cmd = (cmd_generic*)cmd_tanh,
		.shift_hyp_cmd = (cmd_generic*)cmd_atanh,
		.pixel_idx = 3,
		.special_color = NULL
	};
	keypad_map[26] = (kp_key){ // 9
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_9,
		.shift_cmd = NULL,
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 8,
		.special_color = NULL
	};
	keypad_map[27] = (kp_key){ // 6
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_6,
		.shift_cmd = NULL,
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 17,
		.special_color = NULL
	};
	keypad_map[28] = (kp_key){ // 3
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_3,
		.shift_cmd = (cmd_generic*)cmd_pi,
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 22,
		.special_color = NULL
	};
	keypad_map[29] = (kp_key){ // .
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_decimal,
		.shift_cmd = NULL, // TODO: swap
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 31,
		.special_color = NULL
	};
		
	// COL 6 ////////////////////////////
	keypad_map[30] = (kp_key){ // div
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_div,
		.shift_cmd = (cmd_generic*)cmd_inv,
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 4,
		.special_color = NULL
	};
	keypad_map[31] = (kp_key){ // mul
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_mul,
		.shift_cmd = (cmd_generic*)cmd_phasor,
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 7,
		.special_color = NULL
	};
	keypad_map[32] = (kp_key){ // sub
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_sub,
		.shift_cmd = (cmd_generic*)cmd_parallel,
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 18,
		.special_color = NULL
	};
	keypad_map[33] = (kp_key){ // add
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_add,
		.shift_cmd = NULL, // TODO: percent
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 21,
		.special_color = NULL
	};
	keypad_map[34] = (kp_key){0};
		
	// COL 7 ////////////////////////////
	keypad_map[35] = (kp_key){ // hyp
		.key_type = KEY_TYPE_HYP,
		.base_cmd = NULL,
		.shift_cmd = NULL,
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 5,
		.special_color = hyp_key_color
	};
	keypad_map[36] = (kp_key){ // pow
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_pow,
		.shift_cmd = (cmd_generic*)cmd_root,
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 6,
		.special_color = NULL
	};
	keypad_map[37] = (kp_key){ // sqr
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_sqr,
		.shift_cmd = (cmd_generic*)cmd_sqrt,
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 19,
		.special_color = NULL
	};
	keypad_map[38] = (kp_key){ // log
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_log,
		.shift_cmd = (cmd_generic*)cmd_alog,
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 20,
		.special_color = NULL
	};
	keypad_map[39] = (kp_key){ // ln
		.key_type = KEY_TYPE_STD,
		.base_cmd = (cmd_generic*)cmd_ln,
		.shift_cmd = (cmd_generic*)cmd_exp,
		.hyp_cmd = NULL,
		.shift_hyp_cmd = NULL,
		.pixel_idx = 32,
		.special_color = NULL
	};
}
