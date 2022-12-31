/*
 * SSDisplay.h
 *
 * Created: 2022-08-30 11:47:59 AM
 *  Author: Jake Alt
 */ 


#ifndef SSDISPLAY_H_
#define SSDISPLAY_H_
#include <sys/_stdint.h>


typedef struct {
	uint16_t update_period_ticks;
	volatile uint8_t currentDig;
	volatile uint8_t currentData[4];
	uint16_t blankInterval;
} SSDisplay;


void SSD_dispInt(int num);
void SSD_dispStr(const char *text);
void SSD_updateDisp();
void SSD_setBrightness(int brightness);
void SSD_init(SSDisplay* disp_status);


#endif /* SSDISPLAY_H_ */