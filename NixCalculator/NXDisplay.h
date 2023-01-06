/*
 * NXDisplay.h
 *
 * Created: 2022-12-30 11:17:59 PM
 *  Author: Jake Alt
 */ 


#ifndef NXDISPLAY_H_
#define NXDISPLAY_H_

#include <stdint.h>

#define NX_SIGNTUBE_IDX 8
#define NX_NUM_TUBES 9

#define NX_SR_BITS 107
#define NX_SR_BYTES 13

#define NX_DPR 0b100000000000
#define NX_DPL 0b010000000000
#define NUM_TO_NX(x) (((x) == 0) ? (1) : (0b10000000000 >> (x)))
//#define NUM_TO_NX(x) (0b10 << (((x) == 0) ? 10 : (x)))
#define NX_MINUS 0b10;

#define NX_NO_SERCOM 1

typedef struct {
	uint8_t dp_space;
	uint16_t dp_template;
} NXConfig;

void NXDisplay_dispStr(const char *text);
void NXDisplay_init(NXConfig *nxconfig);
#endif /* NXDISPLAY_H_ */