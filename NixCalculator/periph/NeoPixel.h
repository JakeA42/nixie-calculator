/*
 * ws2812 driver
 * Based on code from Luca Davidian
 * http://www.lucadavidian.com/2018/03/08/wifi-controlled-neo-pixels-strips/
 */



#ifndef NEOPIXEL_H_
#define NEOPIXEL_H_

#define NUM_PIXELS   33

#define RGB_COLOR(r, g, b) (((b) << 16) | ((r) << 8) | (g))

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	KITT, BREATHING, BLINK, SOUND,
} Animation;

void NeoPixel_init(void);
void NeoPixel_set_pixel(int pixel, uint8_t red, uint8_t green, uint8_t blue);
void NeoPixel_clear_pixels(void);
void NeoPixel_update(void);
void NeoPixel_set_animation(Animation animation);
void NeoPixel_update_animation(void);

#ifdef __cplusplus
};
#endif

#endif /* NEOPIXEL_H_ */