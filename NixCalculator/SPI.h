/*
 * SPI controls
 * Based on code from Luca Davidian
 * http://www.lucadavidian.com/2018/03/08/wifi-controlled-neo-pixels-strips/
 */

#ifndef SPI_H_
#define SPI_H_

#ifdef __cplusplus
extern "C" {
#endif

void ws2812_SPI_init(void);
void nxdisp_SPI_init();

#ifdef __cplusplus
};
#endif


#endif /* SPI_H_ */