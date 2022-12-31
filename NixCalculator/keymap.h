/*
 * keymap.h
 *
 * Created: 2022-08-30 12:19:53 PM
 *  Author: Jake Alt
 */ 


#ifndef KEYMAP_H_
#define KEYMAP_H_

#include "types.h"
#include "keypad.h"
#include "colors.h"
#include "Commands/command.h"
#include "Commands/commands.h"

#define KEYPAD_NUM_KEYS 33
#define KEYPAD_GRID_SIZE (KEYPAD_NUM_COLS * KEYPAD_NUM_ROWS)

#define KEY_TYPE_STD 0x01
#define KEY_TYPE_MOD_MASK 0xF0
#define KEY_TYPE_SHIFT 0x10
#define KEY_TYPE_HYP 0x20
#define KEY_TYPE_MENU_PWR 0x08
#define KEY_TYPE_CLR 0x09

typedef color_rgb (*SpecialColorQ)(const calc_state_t *,const sys_state_t *);

typedef struct {
	uint8_t key_type;
	cmd_generic *base_cmd;
	cmd_generic *shift_cmd;
	cmd_generic *hyp_cmd;
	cmd_generic *shift_hyp_cmd;
	int pixel_idx;
	SpecialColorQ special_color;
	} kp_key;
#define KEY_MOD_SHIFT_IDX (1ULL << 9)
#define KEY_MOD_HYP_IDX (1ULL << 35)

kp_key keypad_map[KEYPAD_GRID_SIZE];

void build_keymap();

#endif /* KEYMAP_H_ */