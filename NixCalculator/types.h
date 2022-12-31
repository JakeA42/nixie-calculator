/*
 * state.h
 *
 * Created: 2022-12-20 11:50:54 PM
 *  Author: Jake Alt
 */ 


#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>
#include <complex.h>
//temporary
#define CALC_NUM_TYPE complex long double

#define RPN_STACK_MAX_SIZE 40

enum modifier_state {mod_inactive, mod_active, mod_held};

typedef struct {
	struct {
		uint8_t sign;
		int8_t decimal;
		uint8_t length;
		
		char dispstr[11];
	} main;
	struct {
		uint8_t sign;
		int8_t decimal; // Usually not used
		
		char dispstr[5];
	} sci;
} input_buffer_t;


typedef struct {
	struct {
		CALC_NUM_TYPE stack[RPN_STACK_MAX_SIZE];
		int stack_num_items;
		
	} rpn;
	struct {
		//op_cache;
	} infix;
} calc_state_t;

typedef struct {
	struct {
		uint8_t grad_enabled;
		uint8_t dp_disp_style;
		//uint8_t 
	} settings;
	struct {
		enum {au_deg, au_rad, au_grad} angle_units;
		enum {sn_norm, sn_sci, sn_eng} sci_notation;
		enum {em_std, em_rpn} entry_mode;
		struct {
			enum modifier_state shift;
			enum modifier_state hyp;
		} modifiers;
	} sys;
	struct {
		enum {sci_mant, sci_exp} sci_active;
		enum {cv_re, cv_im, cv_r, cv_theta} cpx_view;
		int view_is_real;
		int menu_active;
		enum {uistate_num_entry, uistate_op_select} ui_state;
	} ui;
} sys_state_t;


#endif /* TYPES_H_ */