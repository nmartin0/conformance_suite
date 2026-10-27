/*
 * atomic_compare_exchange_weak() on an object equal to *expected either
 * succeeds (object takes desired, *expected untouched) or fails
 * spuriously (object and *expected both still hold the old value).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_compare_exchange_strong.006
 */
#define PCAT_ID "XSH.atomic_compare_exchange_strong.006"
#include "pcat.h"
#ifndef PCAT_NONE

#define CASE(tag, ct, mn, mx) \
static void run_##tag(void) \
{ \
	ct vals[4] = { (ct)0, (ct)1, (ct)(mn), (ct)(mx) }; \
	int i, t; \
	for (i = 0; i < 4; i++) { \
		for (t = 0; t < 100; t++) { \
			ct exp = vals[i], des = vals[(i + 1) % 4], now; \
			atomic_##tag obj = vals[i]; \
			atomic_##tag *volatile p = &obj; \
			ct *volatile pe = &exp; \
			_Bool r = atomic_compare_exchange_weak(p, pe, des); \
			now = *p; \
			if (r) { \
				pc_check(now == des, "atomic_" #tag ": success left %llx in the object, not the desired %llx", \
				    PCAT_V(now), PCAT_V(des)); \
				pc_check(exp == vals[i], "atomic_" #tag ": *expected changed to %llx on success", PCAT_V(exp)); \
			} else { \
				pc_check(now == vals[i], "atomic_" #tag ": spurious failure changed the object to %llx", \
				    PCAT_V(now)); \
				pc_check(exp == vals[i], "atomic_" #tag ": spurious failure set *expected to %llx, not the old %llx", \
				    PCAT_V(exp), PCAT_V(vals[i])); \
			} \
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
