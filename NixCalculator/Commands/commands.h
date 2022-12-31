/*
 * commands.h
 *
 * Created: 2022-12-20 10:15:34 PM
 *  Author: Jake Alt
 */ 


#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "command.h"

// NUMBERS //////////////////
const cmd_numeric * const cmd_0;
const cmd_numeric * const cmd_1;
const cmd_numeric * const cmd_2;
const cmd_numeric * const cmd_3;
const cmd_numeric * const cmd_4;
const cmd_numeric * const cmd_5;
const cmd_numeric * const cmd_6;
const cmd_numeric * const cmd_7;
const cmd_numeric * const cmd_8;
const cmd_numeric * const cmd_9;
const cmd_numeric * const cmd_decimal;

const cmd_numeric * const cmd_i; // TODO: maybe cmd_op??
const cmd_numeric * const cmd_e; // TODO: maybe cmd_op??
const cmd_numeric * const cmd_pi; // TODO: maybe cmd_op??

// TODO: needs to be a hybrid command...
const cmd_numeric * const cmd_chs;

// MODIFIER /////////////////
//const cmd_op * const cmd_shift;
//const cmd_op * const cmd_hyp;

// VIEW /////////////////////
const cmd_op * const cmd_re_im;
const cmd_op * const cmd_rec_pol;
const cmd_op * const cmd_drg_reint;
const cmd_op * const cmd_drg_convert;


// MATH /////////////////////
const cmd_op * const cmd_add;
const cmd_op * const cmd_sub;
const cmd_op * const cmd_mul;
const cmd_op * const cmd_div;

const cmd_op * const cmd_sin;
const cmd_op * const cmd_cos;
const cmd_op * const cmd_tan;
const cmd_op * const cmd_asin;
const cmd_op * const cmd_acos;
const cmd_op * const cmd_atan;

const cmd_op * const cmd_sinh;
const cmd_op * const cmd_cosh;
const cmd_op * const cmd_tanh;
const cmd_op * const cmd_asinh;
const cmd_op * const cmd_acosh;
const cmd_op * const cmd_atanh;

const cmd_op * const cmd_inv;
const cmd_op * const cmd_phasor;
const cmd_op * const cmd_parallel;

const cmd_op * const cmd_pow;
const cmd_op * const cmd_root;
const cmd_op * const cmd_sqr;
const cmd_op * const cmd_sqrt;

const cmd_op * const cmd_log;
const cmd_op * const cmd_alog;
const cmd_op * const cmd_ln;
const cmd_op * const cmd_exp;

const cmd_op * const cmd_abs;



#endif /* COMMANDS_H_ */