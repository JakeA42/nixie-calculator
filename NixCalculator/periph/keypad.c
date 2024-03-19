/*
 * keypad.c
 *
 * Created: 2022-08-30 12:29:56 PM
 *  Author: Jake Alt
 */ 

#include "keypad.h"
#include "gpio.h"
#include "pins.h"



keypad *keypad_inst = 0;

const unsigned long colPins[]  = {
	KEYPAD_COLA_PIN, KEYPAD_COLB_PIN,
	KEYPAD_COLC_PIN, KEYPAD_COLD_PIN,
	KEYPAD_COLE_PIN, KEYPAD_COLF_PIN,
	KEYPAD_COLG_PIN, KEYPAD_COLH_PIN };

const unsigned long colPorts[] = {
	KEYPAD_COLA_PORT, KEYPAD_COLB_PORT,
	KEYPAD_COLC_PORT, KEYPAD_COLD_PORT,
	KEYPAD_COLE_PORT, KEYPAD_COLF_PORT,
	KEYPAD_COLG_PORT, KEYPAD_COLH_PORT };

const unsigned long rowPins[]  = {
	KEYPAD_ROW0_PIN, KEYPAD_ROW1_PIN,
	KEYPAD_ROW2_PIN, KEYPAD_ROW3_PIN,
	KEYPAD_ROW4_PIN };

const unsigned long rowPorts[] = {
	KEYPAD_ROW0_PORT, KEYPAD_ROW1_PORT,
	KEYPAD_ROW2_PORT, KEYPAD_ROW3_PORT,
	KEYPAD_ROW4_PORT };


uint64_t keypad_get_presses() {
	static uint64_t previous_scan = 0ULL;
	uint64_t newkeys = keypad_inst->rawKeys & (~previous_scan);
	previous_scan = keypad_inst->rawKeys;
	return newkeys;
}

void keypad_scan_keys() {
	keypad_inst->rawKeys = 0ULL;
	gpio_set_pin(colPorts[0], colPins[0], 0);
	for (int col = 0; col < KEYPAD_NUM_COLS; col++) {
		gpio_set_pin(colPorts[col], colPins[col], 1);
		
		for (int row = 0; row < KEYPAD_NUM_ROWS; row++) {
			int key=gpio_read_pin(rowPorts[row], rowPins[row]);
			keypad_inst->rawKeys |= (key ? (1ULL << (row + col * 5)) : 0ULL);
		}
		gpio_set_pin(colPorts[col], colPins[col], 0);
	}
	keypad_inst->changed |= keypad_inst->rawKeys & (~keypad_inst->previous);
	keypad_inst->previous = keypad_inst->rawKeys;
}

void keypad_init(keypad *status) {
	keypad_inst = status;
	for (int i = 0; i < KEYPAD_NUM_COLS; i++) {
		gpio_set_pin_dir(colPorts[i], colPins[i], GPIO_DIR_OUT);
	}
	for (int i = 0; i < KEYPAD_NUM_ROWS; i++) {
		gpio_set_pin_dir(rowPorts[i], rowPins[i], GPIO_DIR_IN);
		gpio_set_pin_pull(rowPorts[i], rowPins[i], GPIO_PULL_DOWN);
	}
	keypad_inst->rawKeys = 0;
	keypad_inst->changed = 0;
	keypad_inst->previous = 0;
} //Keypad
