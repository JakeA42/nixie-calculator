/*
 * gpio.c
 *
 * Created: 2021-08-01 1:46:41 PM
 *  Author: Jake Alt
 */ 


#include "gpio.h"
#include "pins.h"
#include "samd21.h"

void gpio_toggle_pin(unsigned int port, unsigned long pin) {
	if (port == PORTA) {
		REG_PORT_OUTTGL0 = 1 << pin;
	} else if (port == PORTB) {
		REG_PORT_OUTTGL1 = 1 << pin;
	}
}
void gpio_set_pin(unsigned int port, unsigned long pin, int val) {
	if (port == PORTA) {
		if (val)
			REG_PORT_OUTSET0 = 1 << pin;
		else
			REG_PORT_OUTCLR0 = 1 << pin;
	} else if (port == PORTB) {
		if (val)
			REG_PORT_OUTSET1 = 1 << pin;
		else
			REG_PORT_OUTCLR1 = 1 << pin;
	}
}

void gpio_set_pin_dir(unsigned int port, unsigned long pin, int val) {
	
	if (port == PORTA) {
		if (val == GPIO_DIR_OUT) {
			REG_PORT_DIRSET0 = 1 << pin;
			PORT->Group[port].PINCFG[pin].bit.INEN = 0;
		} else {
			REG_PORT_DIRCLR0 = 1 << pin;
			PORT->Group[port].PINCFG[pin].bit.INEN = 1;
		} 
		} else if (port == PORTB) {
		if (val == GPIO_DIR_OUT) {
			REG_PORT_DIRSET1 = 1 << pin;
			PORT->Group[port].PINCFG[pin].bit.INEN = 0;
		} else {
			REG_PORT_DIRCLR1 = 1 << pin;
			PORT->Group[port].PINCFG[pin].bit.INEN = 1;
		}
	}

}
void gpio_set_pin_drvstr(unsigned int port, unsigned long pin, int val) {
	if (port != PORTA && port != PORTB) {
		return;
	}
	PORT->Group[port].PINCFG[pin].bit.DRVSTR = (val ? 1 : 0);
}
void gpio_set_pin_pull(unsigned int port, unsigned long pin, int val) {
	if (port != PORTA && port != PORTB) {
		return;
	}
	if (val == GPIO_PULL_DOWN) {
		
		PORT->Group[port].PINCFG[pin].bit.PULLEN = 1;
		PORT->Group[port].OUTCLR.reg = 1 << pin;
	} else if (val == GPIO_PULL_UP) {
		PORT->Group[port].PINCFG[pin].bit.PULLEN = 1;
		PORT->Group[port].OUTSET.reg = 1 << pin;
	} else {
		PORT->Group[port].PINCFG[pin].bit.PULLEN = 0;
	}
}


int gpio_read_pin(unsigned int port, unsigned long pin) {
	if (port == PORTA) {
		return (PORT->Group[0].IN.reg & (1 << pin)) ? 1 : 0;
	} else if (port == PORTB) {
		return (PORT->Group[1].IN.reg & (1 << pin)) ? 1 : 0;
	}
	return -1;
}
