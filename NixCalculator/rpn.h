/*
 * rpn.h
 *
 * Created: 2022-12-21 12:26:11 AM
 *  Author: Jake Alt
 */ 


#ifndef RPN_H_
#define RPN_H_

#include "types.h"

int rpn_push(calc_state_t * state, CALC_NUM_TYPE value);
int rpn_pop(calc_state_t * state, CALC_NUM_TYPE * value);
int rpn_roll(calc_state_t * state, int amount);




#endif /* RPN_H_ */