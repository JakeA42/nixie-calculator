/*
 * pins.h
 *
 * Created: 2021-08-01 10:48:25 AM
 *  Author: Jake Alt
 */ 


#ifndef PINS_H_
#define PINS_H_

#include "samd21.h"

#define PORTA 0
#define PORTB 1

#define PWR_LED_PORT PORTB
#define PWR_LED_PIN  22
#define BUSY_LED_PORT PORTB
#define BUSY_LED_PIN  23

#define PWR_CTRL_PORT PORTB
#define PWR_CTRL_PIN  17
#define PWR_VSENSE_PORT PORTB
#define PWR_VSENSE_PIN  4


#define SSD_DIG0_PORT PORTB
#define SSD_DIG0_PIN  3
#define SSD_DIG1_PORT PORTB
#define SSD_DIG1_PIN  2
#define SSD_DIG2_PORT PORTB
#define SSD_DIG2_PIN  1
#define SSD_DIG3_PORT PORTB
#define SSD_DIG3_PIN  0

#define SSD_SEG_PORT PORTA
#define SSD_SEGA_PIN 16
#define SSD_SEGB_PIN 17
#define SSD_SEGC_PIN 18
#define SSD_SEGD_PIN 19
#define SSD_SEGE_PIN 21
#define SSD_SEGF_PIN 15
#define SSD_SEGG_PIN 22
#define SSD_SEGDP_PIN 23

#define KEYPAD_COLA_PORT PORTB
#define KEYPAD_COLA_PIN  6
#define KEYPAD_COLB_PORT PORTB
#define KEYPAD_COLB_PIN  7
#define KEYPAD_COLC_PORT PORTB
#define KEYPAD_COLC_PIN  8
#define KEYPAD_COLD_PORT PORTB
#define KEYPAD_COLD_PIN  9
#define KEYPAD_COLE_PORT PORTA
#define KEYPAD_COLE_PIN  4
#define KEYPAD_COLF_PORT PORTA
#define KEYPAD_COLF_PIN  5
#define KEYPAD_COLG_PORT PORTA
#define KEYPAD_COLG_PIN  6
#define KEYPAD_COLH_PORT PORTA
#define KEYPAD_COLH_PIN  7

#define KEYPAD_ROW0_PORT PORTA
#define KEYPAD_ROW0_PIN  10
#define KEYPAD_ROW1_PORT PORTA
#define KEYPAD_ROW1_PIN  11
#define KEYPAD_ROW2_PORT PORTB
#define KEYPAD_ROW2_PIN  10
#define KEYPAD_ROW3_PORT PORTB
#define KEYPAD_ROW3_PIN  11
#define KEYPAD_ROW4_PORT PORTB
#define KEYPAD_ROW4_PIN  12



/* Backlight */
#define BACKLIGHT_SERCOM SERCOM2
#define BACKLIGHT_MOSI_PAD	0
#define BACKLIGHT_MOSI_PORT PORTA
#define BACKLIGHT_MOSI_PIN  12
#define BACKLIGHT_MOSI_PMUX MUX_PA12C_SERCOM2_PAD0

/* NXDisp */
#define NXD_SERCOM SERCOM5
#define NXD_MOSI_PAD  0
#define NXD_MOSI_PORT PORTB
#define NXD_MOSI_PIN  30
#define NXD_MOSI_PMUX MUX_PB30D_SERCOM5_PAD0
#define NXD_SCK_PAD  1
#define NXD_SCK_PORT PORTB
#define NXD_SCK_PIN  31
#define NXD_SCK_PMUX MUX_PB31D_SERCOM5_PAD1
#define NXD_STR_PORT PORTB
#define NXD_STR_PIN  5



#endif /* PINS_H_ */