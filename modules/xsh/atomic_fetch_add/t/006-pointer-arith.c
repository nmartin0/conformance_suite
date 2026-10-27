/*
 * atomic_fetch_add() and atomic_fetch_sub() on atomic pointer objects
 * move the pointer by whole objects of the pointed-to type (pointer
 * arithmetic), return the previous pointer, and accept negative operands.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_fetch_add.006
 */
#define PCAT_ID "XSH.atomic_fetch_add.006"
#include "pcat.h"
#ifndef PCAT_NONE

struct big { char c[24]; };

#define PCASE(name, T) \
static T name##_arr[12]; \
static void run_##name(void) \
{ \
	_Atomic(T *) obj = &name##_arr[2]; \
	_Atomic(T *) *volatile p = &obj; \
	T *old, *now; \
	old = atomic_fetch_add(p, 3); \
	now = *p; \
	pc_check(old == &name##_arr[2], #name ": add returned the wrong previous pointer"); \
	pc_check(now == &name##_arr[5], #name ": adding 3 moved the pointer by %ld bytes, not 3 objects (%lu bytes)", \
	    (long)((char *)now - (char *)&name##_arr[2]), (unsigned long)(3 * sizeof(T))); \
	old = atomic_fetch_sub(p, 2); \
	now = *p; \
	pc_check(old == &name##_arr[5], #name ": sub returned the wrong previous pointer"); \
	pc_check(now == &name##_arr[3], #name ": subtracting 2 left the pointer %ld bytes from the start, expected %lu", \
	    (long)((char *)now - (char *)&name##_arr[0]), (unsigned long)(3 * sizeof(T))); \
	old = atomic_fetch_add(p, -1); \
	now = *p; \
	pc_check(old == &name##_arr[3] && now == &name##_arr[2], #name ": adding -1 gave the wrong pointer"); \
	old = atomic_fetch_sub(p, -4); \
	now = *p; \
	pc_check(old == &name##_arr[2] && now == &name##_arr[6], #name ": subtracting -4 gave the wrong pointer"); \
	old = atomic_fetch_add(p, 0); \
	now = *p; \
	pc_check(old == &name##_arr[6] && now == &name##_arr[6], #name ": adding 0 changed the pointer"); \
	old = atomic_fetch_add_explicit(p, 5, memory_order_acq_rel); \
	now = *p; \
	pc_check(old == &name##_arr[6] && now == &name##_arr[11], #name ": explicit add gave the wrong pointer"); \
	old = atomic_fetch_sub_explicit(p, 11, memory_order_relaxed); \
	now = *p; \
	pc_check(old == &name##_arr[11] && now == &name##_arr[0], #name ": explicit sub gave the wrong pointer"); \
}
PCASE(chr, char)
PCASE(shr, short)
PCASE(num, int)
PCASE(lng, long)
PCASE(dbl, double)
PCASE(big, struct big)

int main(void)
{
	pc_begin(PCAT_ID);
	pc_test();
	run_chr();
	run_shr();
	run_num();
	run_lng();
	run_dbl();
	run_big();
	pc_pass();
}
#endif
