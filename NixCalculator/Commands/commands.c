/*
 * commands.c
 *
 * Created: 2022-12-20 11:46:28 PM
 *  Author: Jake Alt
 */ 

#include "command.h"
#include "../types.h"

static int binary_op_availableQ(const calc_state_t * calc_state, const sys_state_t * sys_state) {
	return 1; // TODO: implement availability
}
static int unary_op_availableQ(const calc_state_t * calc_state, const sys_state_t * sys_state) {
	return 0; // TODO: implement availability
}
static int available_always(const calc_state_t * calc_state, const sys_state_t * sys_state) {
	return 1; // TODO: implement availability
}

static int cmd_re_im_func(calc_state_t * calc_state, sys_state_t * sys_state) {
	switch (sys_state->ui.cpx_view) {
		case cv_re:
			sys_state->ui.cpx_view = cv_im;
			break;
		case cv_im:
			sys_state->ui.cpx_view = cv_re;
			break;
		case cv_r:
			sys_state->ui.cpx_view = cv_theta;
			break;
		case cv_theta:
			sys_state->ui.cpx_view = cv_r;
			break;
	}
	return 0;
}
const cmd_op * const cmd_re_im = &(cmd_op) {
	.cmd_type = CMD_TYPE_OP | CMD_SUBTYPE_VIEWMOD,
	.available = available_always,
	.num_operands = 0,
	.op_func = cmd_re_im_func
};

static int cmd_rec_pol_func(calc_state_t * calc_state, sys_state_t * sys_state) {
	switch (sys_state->ui.cpx_view) {
		case cv_re:
			sys_state->ui.cpx_view = cv_r;
			break;
		case cv_im:
			sys_state->ui.cpx_view = cv_r;
			break;
		case cv_r:
			sys_state->ui.cpx_view = cv_re;
			break;
		case cv_theta:
			sys_state->ui.cpx_view = cv_re;
			break;
	}
	return 0;
}
const cmd_op * const cmd_rec_pol = &(cmd_op) {
	.cmd_type = CMD_TYPE_OP | CMD_SUBTYPE_VIEWMOD,
	.available = available_always,
	.num_operands = 0,
	.op_func = cmd_rec_pol_func
};

static int cmd_reint_drg_func(calc_state_t * calc_state, sys_state_t * sys_state) {
	switch (sys_state->sys.angle_units) {
		case au_deg:
			sys_state->sys.angle_units = au_rad;
			break;
		case au_rad:
			sys_state->sys.angle_units = sys_state->settings.grad_enabled ? au_grad : au_deg;
			break;
		case au_grad:
			sys_state->sys.angle_units = au_deg;
			break;
	}
	return 0;
}
const cmd_op * const cmd_drg_reint = &(cmd_op) {
	.cmd_type = CMD_TYPE_OP | CMD_SUBTYPE_VIEWMOD,
	.available = available_always,
	.num_operands = 0,
	.op_func = cmd_reint_drg_func
};



static int cmd_add_func(calc_state_t * calc_state, sys_state_t * sys_state) {
	if (sys_state->sys.entry_mode == em_rpn) {
		calc_state->rpn.stack[0] = calc_state->rpn.stack[0] + calc_state->rpn.stack[1];
	} else {
		
	}
	return 0;
}
const cmd_op * const cmd_add = &(cmd_op) {
	.cmd_type = CMD_TYPE_OP | CMD_SUBTYPE_MATH_OP,
	.available = binary_op_availableQ,
	.num_operands = 2,
	.op_func = cmd_add_func
};
const cmd_op * const cmd_sub = &(cmd_op) {
	.cmd_type = CMD_TYPE_OP | CMD_SUBTYPE_MATH_OP,
	.available = binary_op_availableQ,
	.num_operands = 2,
	.op_func = cmd_add_func
};
const cmd_op * const cmd_mul = &(cmd_op) {
	.cmd_type = CMD_TYPE_OP | CMD_SUBTYPE_MATH_OP,
	.available = binary_op_availableQ,
	.num_operands = 2,
	.op_func = cmd_add_func
};



const cmd_op * const cmd_sin = &(cmd_op) {
	.cmd_type = CMD_TYPE_OP | CMD_SUBTYPE_MATH_OP,
	.available = unary_op_availableQ,
	.num_operands = 1,
	.op_func = cmd_add_func
};
const cmd_op * const cmd_sinh = &(cmd_op) {
	.cmd_type = CMD_TYPE_OP | CMD_SUBTYPE_MATH_OP,
	.available = binary_op_availableQ,
	.num_operands = 1,
	.op_func = cmd_add_func
};
const cmd_op * const cmd_asin = &(cmd_op) {
	.cmd_type = CMD_TYPE_OP | CMD_SUBTYPE_MATH_OP,
	.available = binary_op_availableQ,
	.num_operands = 1,
	.op_func = cmd_add_func
};
const cmd_op * const cmd_asinh = &(cmd_op) {
	.cmd_type = CMD_TYPE_OP | CMD_SUBTYPE_MATH_OP,
	.available = binary_op_availableQ,
	.num_operands = 1,
	.op_func = cmd_add_func
};
