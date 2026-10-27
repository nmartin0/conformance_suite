/*
 * atomic_compare_exchange_strong() on an object equal to *expected stores
 * desired, returns true, and leaves *expected alone; every atomic integer
 * type.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_compare_exchange_strong.001
 */
#define PCAT_ID "XSH.atomic_compare_exchange_strong.001"
#include "pcat.h"
#ifndef PCAT_NONE

#define CASE(tag, ct, mn, mx) \
static void run_##tag(void) \
{ \
	ct vals[4] = { (ct)0, (ct)1, (ct)(mn), (ct)(mx) }; \
	int i; \
	for (i = 0; i < 4; i++) { \
		ct exp = vals[i], des = vals[(i + 1) % 4], now; \
		atomic_##tag obj = vals[i]; \
		atomic_##tag *volatile p = &obj; \
		ct *volatile pe = &exp; \
		_Bool r = atomic_compare_exchange_strong(p, pe, des); \
		now = *p; \
		pc_check(r, "atomic_" #tag ": returned false for equal values (%llx)", PCAT_V(vals[i])); \
		pc_check(now == des, "atomic_" #tag ": object holds %llx, expected the desired %llx", \
		    PCAT_V(now), PCAT_V(des)); \
		pc_check(exp == vals[i], "atomic_" #tag ": *expected changed to %llx on success", PCAT_V(exp)); \
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
