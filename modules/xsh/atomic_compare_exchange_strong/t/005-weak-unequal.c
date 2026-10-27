/*
 * atomic_compare_exchange_weak() on an object that differs from
 * *expected never succeeds: it returns false, leaves the object alone and
 * stores the object's value in *expected; every atomic integer type.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_compare_exchange_strong.005
 */
#define PCAT_ID "XSH.atomic_compare_exchange_strong.005"
#include "pcat.h"
#ifndef PCAT_NONE

#define CASE(tag, ct, mn, mx) \
static void run_##tag(void) \
{ \
	ct vals[4] = { (ct)0, (ct)1, (ct)(mn), (ct)(mx) }; \
	int i, t; \
	for (i = 0; i < 4; i++) { \
		for (t = 0; t < 50; t++) { \
			ct exp = vals[(i + 1) % 4], des = vals[(i + 2) % 4], now; \
			atomic_##tag obj = vals[i]; \
			atomic_##tag *volatile p = &obj; \
			ct *volatile pe = &exp; \
			_Bool r = atomic_compare_exchange_weak(p, pe, des); \
			now = *p; \
			pc_check(!r, "atomic_" #tag ": weak exchange returned true for different values"); \
			pc_check(now == vals[i], "atomic_" #tag ": object changed to %llx (was %llx)", \
			    PCAT_V(now), PCAT_V(vals[i])); \
			pc_check(exp == vals[i], "atomic_" #tag ": *expected is %llx, expected the object's %llx", \
			    PCAT_V(exp), PCAT_V(vals[i])); \
		} \
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
