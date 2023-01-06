/*
 * command.h
 *
 * Created: 2022-12-20 10:16:05 PM
 *  Author: Jake Alt
 */ 


#ifndef COMMAND_H_
#define COMMAND_H_

#include <stdint.h>
#include "../types.h"

#define CMD_TYPE_MASK	0xF0
#define CMD_TYPE_OP		0x10
#define CMD_TYPE_MENU	0x20
#define CMD_TYPE_NUM	0x30
#define CMD_TYPE_CLR	0x40
#define CMD_TYPE_MOD	0x50

#define CMD_SUBTYPE_MATH_OP 0x01
#define CMD_SUBTYPE_VIEWMOD 0x02
#define CMD_SUBTYPE_CONST 0x03


//                   registers, state
typedef int (*Operation)(calc_state_t *,sys_state_t *);

//						  state
typedef int (*AvailableQ)(const calc_state_t *, const sys_state_t *);

//						   registers, state, number
typedef int (*NumericFunc)(calc_state_t *,sys_state_t *,int);




typedef struct {
	uint8_t cmd_type;
	AvailableQ available;
	} cmd_generic;

// Operations, view modifiers, constants
typedef struct {
	uint8_t cmd_type;
	AvailableQ available;
	int num_operands;
	Operation op_func;
	} cmd_op;

typedef struct {
	uint8_t cmd_type;
	AvailableQ available;
	/*TBD menu pointer */
	} cmd_menu;

typedef struct {
	uint8_t cmd_type;
	AvailableQ available;
	int number;
	NumericFunc num_func;
	} cmd_numeric;

#endif /* COMMAND_H_ */