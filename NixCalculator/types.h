/*
 * state.h
 *
 * Created: 2022-12-20 11:50:54 PM
 *  Author: Jake Alt
 */ 


#ifndef TYPES_H_
#define TYPES_H_

#include <stdbool.h>
#include <stdint.h>
#include <complex.h>
#include "mathcore/mathcore.h"
// TODO: #include <quadmath.h> __complex128
//temporary
//#define CALC_NUM_TYPE complex long double
#define CALC_NUM_TYPE rnum_t

#define RPN_STACK_MAX_SIZE 40

enum modifier_state {mod_inactive, mod_active, mod_held};

typedef struct {
	struct {
		int8_t sign;
		int8_t decimal;
		uint8_t length;
		int8_t nums[8];
		//char dispstr[11];
	} main;
	struct {
		uint8_t sign;
		int8_t decimal; // Usually not used
		
		char dispstr[5];
	} sci;
	enum {ent_none, ent_normal, ent_sci} entry_field;
} input_buffer_t;
// TODO: change this to use an input buffer pointer and have pointers to separate input buffers for normal, scientific,  other...

typedef struct {
	int (*unary_fn)(CALC_NUM_TYPE*, const CALC_NUM_TYPE*);
	int (*binary_fn)(CALC_NUM_TYPE*, const CALC_NUM_TYPE*, const CALC_NUM_TYPE*);
	CALC_NUM_TYPE *opa;
	CALC_NUM_TYPE *opb;
	CALC_NUM_TYPE *opx;
	int8_t num_operands;
	bool active;
} op_cache_t;

typedef struct {
	struct {
		CALC_NUM_TYPE stack[RPN_STACK_MAX_SIZE];
		int stack_num_items;
		
	} rpn;
	struct {
		op_cache_t op_cache;
		CALC_NUM_TYPE reg1;
		CALC_NUM_TYPE reg2;
		
	} infix;
	input_buffer_t in_buffer;
	bool entry_in_progress;
	
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
	} sys;
	struct {
		enum modifier_state shift;
		enum modifier_state hyp;
	} mods;
	struct {
		enum {sci_mant, sci_exp} sci_active;
		enum {cv_re, cv_im, cv_r, cv_theta} cpx_view;
		int view_is_real;
		int menu_active;
		enum {uistate_num_entry, uistate_op_select} ui_state;
	} ui;
} sys_state_t;


#endif /* TYPES_H_ */