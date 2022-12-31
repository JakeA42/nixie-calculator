/*
 * colors.h
 *
 * Created: 2022-12-21 12:50:28 PM
 *  Author: Jake Alt
 */ 


#ifndef COLORS_H_
#define COLORS_H_

#include <stdint.h>
typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} color_rgb;

const color_rgb color_off;
const color_rgb color_normal;
const color_rgb color_shift;
const color_rgb color_hyp;
const color_rgb color_shift_hyp;
const color_rgb color_normal_unavail;
const color_rgb color_shift_unavail;
const color_rgb color_hyp_unavail;
const color_rgb color_shift_hyp_unavail;

const color_rgb color_cplx_re;
const color_rgb color_cplx_im;
const color_rgb color_cplx_r;
const color_rgb color_cplx_theta;

const color_rgb color_drg_d;
const color_rgb color_drg_r;
const color_rgb color_drg_g;

const color_rgb color_clear;
const color_rgb color_menu;


#endif /* COLORS_H_ */