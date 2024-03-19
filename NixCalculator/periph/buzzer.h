/*
 * buzzer.h
 *
 * Created: 2022-12-31 9:38:02 PM
 *  Author: Jake Alt
 */ 


#ifndef BUZZER_H_
#define BUZZER_H_

#define BUZZER_TONE_GOOD() buzzer_tone_start(16, 1800)
#define BUZZER_TONE_BAD() buzzer_tone_start(80, 600)

void buzzer_tone_start(uint16_t duration_ms, uint16_t freq_hz);

void buzzer_timer_tick();
void buzzer_init();


#endif /* BUZZER_H_ */