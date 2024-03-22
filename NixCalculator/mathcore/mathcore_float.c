/*
 * mathcore.c
 *
 * Created: 2024-03-18 6:00:17 PM
 *  Author: Jake Alt
 */ 

#include "mathcore.h"

#ifdef MATH_DOUBLE

int r_add(rnum_t *x, const rnum_t *a, const rnum_t *b) {
	x->val = a->val + b->val;
	return 0;
}
int r_sub(rnum_t *x, const rnum_t *a, const rnum_t *b) {
	x->val = a->val - b->val;
	return 0;
}
int r_mul(rnum_t *x, const rnum_t *a, const rnum_t *b) {
	x->val = a->val * b->val;
	return 0;
}
int r_div(rnum_t *x, const rnum_t *a, const rnum_t *b) {
	if (b->val == 0.0) {
		return -1;
	}
	x->val = a->val / b->val;
	return 0;
}



#endif