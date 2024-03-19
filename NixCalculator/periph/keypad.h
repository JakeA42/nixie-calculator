/*
 * keypad.h
 *
 * Created: 2022-08-30 12:30:03 PM
 *  Author: Jake Alt
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_
#include <sys/_stdint.h>

#define KEYPAD_NUM_COLS 8
#define KEYPAD_NUM_ROWS 5

typedef struct {
	uint16_t update_rate_ticks;
	volatile uint64_t rawKeys;
	volatile uint64_t changed;
	volatile uint64_t previous;
} keypad;

void keypad_scan_keys();

uint64_t keypad_get_presses();
//uint64_t keypad_get_pressed_keys();
//uint64_t keypad_get_held_keys();
void keypad_init(keypad *status);


#endif /* KEYPAD_H_ */