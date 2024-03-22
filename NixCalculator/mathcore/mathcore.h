/*
 * mathcore.h
 *
 * Created: 2024-03-18 6:00:29 PM
 *  Author: Jake Alt
 */ 


#ifndef MATHCORE_H_
#define MATHCORE_H_

#define MATH_DOUBLE


#ifdef MATH_DOUBLE
typedef struct {
	double val;
} rnum_t;

#endif
	
int r_add(rnum_t *x, const rnum_t *a, const rnum_t *b);
int r_sub(rnum_t *x, const rnum_t *a, const rnum_t *b);
int r_mul(rnum_t *x, const rnum_t *a, const rnum_t *b);
int r_div(rnum_t *x, const rnum_t *a, const rnum_t *b);


#endif /* MATHCORE_H_ */