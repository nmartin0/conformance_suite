/*
 * A weak compare-and-exchange in a loop, with equal values, succeeds
 * within a million attempts (the page expects weak exchanges to be used
 * in loops).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_compare_exchange_strong.007
 */
#define PCAT_ID "XSH.atomic_compare_exchange_strong.007"
#include "pcat.h"
#ifndef PCAT_NONE

#define CASE(tag, ct, mn, mx) \
static void run_##tag(void) \
{ \
	ct vals[4] = { (ct)0, (ct)1, (ct)(mn), (ct)(mx) }; \
	int i, tries; \
	for (i = 0; i < 4; i++) { \
		ct exp = vals[i], des = vals[(i + 1) % 4], now; \
		atomic_##tag obj = vals[i]; \
		atomic_##tag *volatile p = &obj; \
		ct *volatile pe = &exp; \
		_Bool r = 0; \
		for (tries = 0; tries < 1000000 && !r; tries++) \
			r = atomic_compare_exchange_weak(p, pe, des); \
		now = *p; \
		pc_check(r, "atomic_" #tag ": no success in a million weak attempts"); \
		pc_check(now == des, "atomic_" #tag ": object holds %llx, expected %llx", PCAT_V(now), PCAT_V(des)); \
	} \
}
PCAT_ALL(CASE)
#undef CASE

int main(void)
{
	pc_begin(PCAT_ID);
	pc_test();
#define CALL(tag, ct, mn, mx) run_##tag();
	PCAT_ALL(CALL)
#undef CALL
	pc_pass();
}
#endif
