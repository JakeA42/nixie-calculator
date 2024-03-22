/*
 * SSDisplay.c
 *
 * Created: 2022-08-30 11:47:46 AM
 *  Author: Jake Alt
 */ 

#include "SSDisplay.h"
#include "gpio.h"
#include "pins.h"

#define DISP_SIZE 4

#define SSD_SEG_A  (1 << 0)
#define SSD_SEG_B  (1 << 1)
#define SSD_SEG_C  (1 << 2)
#define SSD_SEG_D  (1 << 3)
#define SSD_SEG_E  (1 << 4)
#define SSD_SEG_F  (1 << 5)
#define SSD_SEG_G  (1 << 6)
#define SSD_SEG_DP (1 << 7)

const uint8_t sciDispNumLUT[10] = {
	SSD_SEG_A	| SSD_SEG_B	| SSD_SEG_C	| SSD_SEG_D	| SSD_SEG_E	| SSD_SEG_F	| 0,			// 0
	0			| SSD_SEG_B	| SSD_SEG_C	| 0			| 0			| 0			| 0,			// 1
	SSD_SEG_A	| SSD_SEG_B	| 0			| SSD_SEG_D	| SSD_SEG_E	| 0			| SSD_SEG_G,	// 2
	SSD_SEG_A	| SSD_SEG_B	| SSD_SEG_C	| SSD_SEG_D	| 0			| 0			| SSD_SEG_G,	// 3
	0			| SSD_SEG_B	| SSD_SEG_C	| 0			| 0			| SSD_SEG_F	| SSD_SEG_G,	// 4
	SSD_SEG_A	| 0			| SSD_SEG_C	| SSD_SEG_D	| 0			| SSD_SEG_F	| SSD_SEG_G,	// 5
	SSD_SEG_A	| 0			| SSD_SEG_C	| SSD_SEG_D	| SSD_SEG_E	| SSD_SEG_F	| SSD_SEG_G,	// 6
	SSD_SEG_A	| SSD_SEG_B	| SSD_SEG_C	| 0			| 0			| 0			| 0,			// 7
	SSD_SEG_A	| SSD_SEG_B	| SSD_SEG_C	| SSD_SEG_D	| SSD_SEG_E	| SSD_SEG_F	| SSD_SEG_G,	// 8
	SSD_SEG_A	| SSD_SEG_B	| SSD_SEG_C	| SSD_SEG_D	| 0			| SSD_SEG_F	| SSD_SEG_G		// 9
};

#define SSD_CHAR_MINUS	(SSD_SEG_G)
#define SSD_CHAR_A		(SSD_SEG_A	| SSD_SEG_B	| SSD_SEG_C	| 0			| SSD_SEG_E	| SSD_SEG_F	| SSD_SEG_G)
#define SSD_CHAR_a		(SSD_SEG_A	| SSD_SEG_B	| SSD_SEG_C	| SSD_SEG_D	| SSD_SEG_E	| 0			| SSD_SEG_G)
#define SSD_CHAR_b		(0			| 0			| SSD_SEG_C	| SSD_SEG_D	| SSD_SEG_E	| SSD_SEG_F	| SSD_SEG_G)
#define SSD_CHAR_C		(SSD_SEG_A	| 0			| 0			| SSD_SEG_D	| SSD_SEG_E	| SSD_SEG_F	| 0)
#define SSD_CHAR_c		(0			| 0			| 0			| SSD_SEG_D	| SSD_SEG_E	| 0			| SSD_SEG_G)
#define SSD_CHAR_d		(0			| SSD_SEG_B	| SSD_SEG_C	| SSD_SEG_D	| SSD_SEG_E	| 0			| SSD_SEG_G)
#define SSD_CHAR_E		(SSD_SEG_A	| 0			| 0			| SSD_SEG_D	| SSD_SEG_E	| SSD_SEG_F	| SSD_SEG_G)
#define SSD_CHAR_e		(SSD_SEG_A	| SSD_SEG_B	| 0			| SSD_SEG_D	| SSD_SEG_E	| SSD_SEG_F	| SSD_SEG_G)
#define SSD_CHAR_F		(SSD_SEG_A	| 0			| 0			| 0			| SSD_SEG_E	| SSD_SEG_F	| SSD_SEG_G)
#define SSD_CHAR_G		(SSD_SEG_A	| 0			| SSD_SEG_C	| SSD_SEG_D	| SSD_SEG_E	| SSD_SEG_F	| 0)
#define SSD_CHAR_g		(SSD_SEG_A	| SSD_SEG_B	| SSD_SEG_C	| SSD_SEG_D	| 0			| SSD_SEG_F	| SSD_SEG_G)
#define SSD_CHAR_H		(0			| SSD_SEG_B	| SSD_SEG_C	| 0			| SSD_SEG_E	| SSD_SEG_F	| SSD_SEG_G)
#define SSD_CHAR_h		(0			| 0			| SSD_SEG_C	| 0			| SSD_SEG_E	| SSD_SEG_F	| SSD_SEG_G)
#define SSD_CHAR_I		(SSD_SEG_B	| SSD_SEG_C)
#define SSD_CHAR_i		(SSD_SEG_C)
#define SSD_CHAR_J		(0			| SSD_SEG_B	| SSD_SEG_C	| SSD_SEG_D	| SSD_SEG_E	| 0			| 0)
#define SSD_CHAR_K		(0			| SSD_SEG_B	| 0			| SSD_SEG_D	| SSD_SEG_E	| SSD_SEG_F	| SSD_SEG_G)
#define SSD_CHAR_L		(0			| 0			| 0			| SSD_SEG_D	| SSD_SEG_E	| SSD_SEG_F	| SSD_SEG_G)
#define SSD_CHAR_l		(SSD_SEG_B	| SSD_SEG_C)
#define SSD_CHAR_n		(0			| 0			| SSD_SEG_C	| 0			| SSD_SEG_E	| 0			| SSD_SEG_G)
#define SSD_CHAR_O		(SSD_SEG_A	| SSD_SEG_B	| SSD_SEG_C	| SSD_SEG_D	| SSD_SEG_E	| SSD_SEG_F	| 0)
#define SSD_CHAR_o		(0			| 0			| SSD_SEG_C	| SSD_SEG_D	| SSD_SEG_E	| 0			| SSD_SEG_G)
#define SSD_CHAR_P		(SSD_SEG_A	| SSD_SEG_B	| 0			| 0			| SSD_SEG_E	| 0			| SSD_SEG_G)
#define SSD_CHAR_R		(SSD_SEG_A	| 0			| 0			| 0			| SSD_SEG_E	| SSD_SEG_F	| 0)
#define SSD_CHAR_r		(0			| 0			| 0			| 0			| SSD_SEG_E	| 0			| SSD_SEG_G)
#define SSD_CHAR_S		(SSD_SEG_A	| 0			| SSD_SEG_C	| SSD_SEG_D	| 0			| SSD_SEG_F	| SSD_SEG_G)
#define SSD_CHAR_t		(0			| 0			| 0			| SSD_SEG_D	| SSD_SEG_E	| SSD_SEG_F	| SSD_SEG_G)
#define SSD_CHAR_U		(0			| SSD_SEG_B	| SSD_SEG_C	| SSD_SEG_D	| SSD_SEG_E	| SSD_SEG_F	| 0)
#define SSD_CHAR_u		(0			| 0			| SSD_SEG_C	| SSD_SEG_D	| SSD_SEG_E	| 0			| 0)
#define SSD_CHAR_V		(0			| 0			| SSD_SEG_C	| SSD_SEG_D	| SSD_SEG_E	| 0			| 0)
#define SSD_CHAR_qm		(SSD_SEG_A	| SSD_SEG_B	| 0			| 0			| SSD_SEG_E	| 0			| SSD_SEG_G)



static SSDisplay *ssd = 0;


void SSD_dispStr(const char *text) {
	for (int i = 0; i < DISP_SIZE; i++) {
		ssd->currentData[i] = 0;
	}
	for (int i = 0, dig = DISP_SIZE - 1; dig >= 0 && text[i]; i++, dig--) {
		if (dig < (text[i] == '.' ? -1 : 0)) {
			break;
		}
		if (text[i] >= '0' && text[i] <= '9') {
			ssd->currentData[dig] = sciDispNumLUT[text[i] - '0'];
		} else {
			switch (text[i]) {
				case ' ': ssd->currentData[dig] = 0; break;
				case '.': dig++; ssd->currentData[dig] |= SSD_SEG_DP; break;
				case '-': ssd->currentData[dig] = SSD_CHAR_MINUS; break;
				case 'A': ssd->currentData[dig] = SSD_CHAR_A; break;
				case 'a': ssd->currentData[dig] = SSD_CHAR_a; break;
				case 'b': ssd->currentData[dig] = SSD_CHAR_b; break;
				case 'C': ssd->currentData[dig] = SSD_CHAR_C; break;
				case 'c': ssd->currentData[dig] = SSD_CHAR_c; break;
				case 'd': ssd->currentData[dig] = SSD_CHAR_d; break;
				case 'E': ssd->currentData[dig] = SSD_CHAR_E; break;
				case 'e': ssd->currentData[dig] = SSD_CHAR_e; break;
				case 'F': ssd->currentData[dig] = SSD_CHAR_F; break;
				case 'G': ssd->currentData[dig] = SSD_CHAR_G; break;
				case 'g': ssd->currentData[dig] = SSD_CHAR_g; break;
				case 'H': ssd->currentData[dig] = SSD_CHAR_H; break;
				case 'h': ssd->currentData[dig] = SSD_CHAR_h; break;
				case 'I': ssd->currentData[dig] = SSD_CHAR_I; break;
				case 'i': ssd->currentData[dig] = SSD_CHAR_i; break;
				case 'J': ssd->currentData[dig] = SSD_CHAR_J; break;
				case 'K': ssd->currentData[dig] = SSD_CHAR_K; break;
				case 'L': ssd->currentData[dig] = SSD_CHAR_L; break;
				case 'l': ssd->currentData[dig] = SSD_CHAR_l; break;
				case 'n': ssd->currentData[dig] = SSD_CHAR_n; break;
				case 'O': ssd->currentData[dig] = SSD_CHAR_O; break;
				case 'o': ssd->currentData[dig] = SSD_CHAR_o; break;
				case 'P': ssd->currentData[dig] = SSD_CHAR_P; break;
				case 'R': ssd->currentData[dig] = SSD_CHAR_R; break;
				case 'r': ssd->currentData[dig] = SSD_CHAR_r; break;
				case 'S': ssd->currentData[dig] = SSD_CHAR_S; break;
				case 't': ssd->currentData[dig] = SSD_CHAR_t; break;
				case 'U': ssd->currentData[dig] = SSD_CHAR_U; break;
				case 'u': ssd->currentData[dig] = SSD_CHAR_u; break;
				case 'V': ssd->currentData[dig] = SSD_CHAR_V; break;
				case '?': ssd->currentData[dig] = SSD_CHAR_qm; break;
				default:  ssd->currentData[dig] = 0; break;
			}
		}
	}
}

void SSD_updateDisp() {
	gpio_set_pin(SSD_DIG0_PORT, SSD_DIG0_PIN, 1); // Active low, clear display to prevent ghosting
	gpio_set_pin(SSD_DIG1_PORT, SSD_DIG1_PIN, 1);
	gpio_set_pin(SSD_DIG2_PORT, SSD_DIG2_PIN, 1);
	gpio_set_pin(SSD_DIG3_PORT, SSD_DIG3_PIN, 1);

	gpio_set_pin(SSD_SEG_PORT, SSD_SEGA_PIN, !(ssd->currentData[ssd->currentDig] & SSD_SEG_A));
	gpio_set_pin(SSD_SEG_PORT, SSD_SEGB_PIN, !(ssd->currentData[ssd->currentDig] & SSD_SEG_B));
	gpio_set_pin(SSD_SEG_PORT, SSD_SEGC_PIN, !(ssd->currentData[ssd->currentDig] & SSD_SEG_C));
	gpio_set_pin(SSD_SEG_PORT, SSD_SEGD_PIN, !(ssd->currentData[ssd->currentDig] & SSD_SEG_D));
	gpio_set_pin(SSD_SEG_PORT, SSD_SEGE_PIN, !(ssd->currentData[ssd->currentDig] & SSD_SEG_E));
	gpio_set_pin(SSD_SEG_PORT, SSD_SEGF_PIN, !(ssd->currentData[ssd->currentDig] & SSD_SEG_F));
	gpio_set_pin(SSD_SEG_PORT, SSD_SEGG_PIN, !(ssd->currentData[ssd->currentDig] & SSD_SEG_G));
	gpio_set_pin(SSD_SEG_PORT, SSD_SEGDP_PIN, !(ssd->currentData[ssd->currentDig] & SSD_SEG_DP));

	gpio_set_pin(SSD_DIG0_PORT, SSD_DIG0_PIN, ssd->currentDig != 0); // Active low
	gpio_set_pin(SSD_DIG1_PORT, SSD_DIG1_PIN, ssd->currentDig != 1);
	gpio_set_pin(SSD_DIG2_PORT, SSD_DIG2_PIN, ssd->currentDig != 2);
	gpio_set_pin(SSD_DIG3_PORT, SSD_DIG3_PIN, ssd->currentDig != 3);
	ssd->currentDig++;
	if (ssd->currentDig >= DISP_SIZE + ssd->blankInterval) {
		ssd->currentDig = 0;
	}
}

void SSD_setBrightness(int brightness) {
	
}


void SSDgpioInit() {
	gpio_set_pin_dir(SSD_DIG0_PORT, SSD_DIG0_PIN, GPIO_DIR_OUT); // Active low
	gpio_set_pin_dir(SSD_DIG1_PORT, SSD_DIG1_PIN, GPIO_DIR_OUT);
	gpio_set_pin_dir(SSD_DIG2_PORT, SSD_DIG2_PIN, GPIO_DIR_OUT);
	gpio_set_pin_dir(SSD_DIG3_PORT, SSD_DIG3_PIN, GPIO_DIR_OUT);
	
	gpio_set_pin_drvstr(SSD_SEG_PORT, SSD_SEGA_PIN, 1);
	gpio_set_pin_drvstr(SSD_SEG_PORT, SSD_SEGB_PIN, 1);
	gpio_set_pin_drvstr(SSD_SEG_PORT, SSD_SEGC_PIN, 1);
	gpio_set_pin_drvstr(SSD_SEG_PORT, SSD_SEGD_PIN, 1);
	gpio_set_pin_drvstr(SSD_SEG_PORT, SSD_SEGE_PIN, 1);
	gpio_set_pin_drvstr(SSD_SEG_PORT, SSD_SEGF_PIN, 1);
	gpio_set_pin_drvstr(SSD_SEG_PORT, SSD_SEGG_PIN, 1);
	gpio_set_pin_drvstr(SSD_SEG_PORT, SSD_SEGDP_PIN, 1);

	gpio_set_pin_dir(SSD_SEG_PORT, SSD_SEGA_PIN, GPIO_DIR_OUT);
	gpio_set_pin_dir(SSD_SEG_PORT, SSD_SEGB_PIN, GPIO_DIR_OUT);
	gpio_set_pin_dir(SSD_SEG_PORT, SSD_SEGC_PIN, GPIO_DIR_OUT);
	gpio_set_pin_dir(SSD_SEG_PORT, SSD_SEGD_PIN, GPIO_DIR_OUT);
	gpio_set_pin_dir(SSD_SEG_PORT, SSD_SEGE_PIN, GPIO_DIR_OUT);
	gpio_set_pin_dir(SSD_SEG_PORT, SSD_SEGF_PIN, GPIO_DIR_OUT);
	gpio_set_pin_dir(SSD_SEG_PORT, SSD_SEGG_PIN, GPIO_DIR_OUT);
	gpio_set_pin_dir(SSD_SEG_PORT, SSD_SEGDP_PIN, GPIO_DIR_OUT);
}

void SSD_init(SSDisplay* disp_status) {
	ssd = disp_status;
	ssd->update_period_ticks = 16;
	ssd->currentDig = 0;
	for (int i = 0; i < DISP_SIZE; i++) {
		ssd->currentData[i] = 0;
	}
	ssd->blankInterval = 0;
	SSDgpioInit();
}

