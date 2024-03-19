/*
 * buzzer.c
 *
 * Created: 2022-12-31 9:32:30 PM
 *  Author: Jake Alt
 */ 

#include "samd21.h"
#include "pins.h"
#include "buzzer.h"

static struct {
	uint16_t remaining_time_ms;
	uint16_t freq_hz;
	uint8_t active;
} current_tone = { 0 };

void buzzer_init() {
	// Enable clock for TC6
	PM->APBCMASK.bit.TC6_ = 1;
		
	GCLK->CLKCTRL.bit.ID = GCLK_CLKCTRL_ID_TC6_TC7_Val;
		
	GCLK->CLKCTRL.bit.GEN = GCLK_CLKCTRL_GEN_GCLK0_Val;
	GCLK->CLKCTRL.bit.CLKEN = 1;
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |
						GCLK_CLKCTRL_GEN_GCLK0 |  // source is generic clock generator 0
						GCLK_CLKCTRL_ID_TC6_TC7;  // route to tc6&7
	
	// Setup timer
	TC6->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16; // 16 bit mode
	
	TC6->COUNT16.CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ; // Match Frequency generation
	// 48MHz / 512 = 46875 Hz tick
	TC6->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024;
	// CC0 will control the period
	
	// Set pin for TC6 output
	PORT->Group[BUZZER_PORT].PINCFG[BUZZER_PIN].bit.PMUXEN = 1;
	PORT->Group[BUZZER_PORT].PMUX[BUZZER_PIN/2].bit.PMUXE = BUZZER_PMUX;
		
}

void buzzer_tone_start(uint16_t duration_ms, uint16_t freq_hz) {
	if (freq_hz == 0 || freq_hz > 23438 || duration_ms == 0) {
		return;
	}
	current_tone.remaining_time_ms = duration_ms;
	current_tone.freq_hz = freq_hz;
	current_tone.active = 1;
	// Set timer for waveform generation
	TC6->COUNT16.CC[0].reg = (uint16_t)(23438 / freq_hz);
	
	TC6->COUNT16.CTRLA.bit.ENABLE = 1;
}

void buzzer_timer_tick() {
	if (current_tone.active && --current_tone.remaining_time_ms <= 0) {
		// TODO: Stop tone
		TC6->COUNT16.CTRLA.bit.ENABLE = 0;
		current_tone.active = 0;
	}
}