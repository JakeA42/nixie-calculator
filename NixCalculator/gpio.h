/*
 * gpio.h
 *
 * Created: 2021-08-01 1:43:46 PM
 *  Author: Jake Alt
 */ 


#ifndef GPIO_H_
#define GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO(x) x##_PORT, x##_PIN

#define GPIO_DIR_IN 0
#define GPIO_DIR_OUT 1


#define GPIO_PULL_NONE 0
#define GPIO_PULL_UP   1
#define GPIO_PULL_DOWN 2

void gpio_toggle_pin(unsigned int port, unsigned long pin);
void gpio_set_pin(unsigned int port, unsigned long pin, int val);

void gpio_set_pin_dir(unsigned int port, unsigned long pin, int val);
void gpio_set_pin_drvstr(unsigned int port, unsigned long pin, int val);
void gpio_set_pin_pull(unsigned int port, unsigned long pin, int val);

int gpio_read_pin(unsigned int port, unsigned long pin);

#ifdef __cplusplus
};
#endif


#endif /* GPIO_H_ */