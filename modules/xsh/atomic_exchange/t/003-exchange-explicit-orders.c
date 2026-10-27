/*
 * atomic_exchange_explicit() installs the new value and returns the
 * previous one with every memory order, for every atomic integer type.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_exchange.003
 */
#define PCAT_ID "XSH.atomic_exchange.003"
#include "pcat.h"
#ifndef PCAT_NONE

#define CASE(tag, ct, mn, mx) \
static void run_##tag(void) \
{ \
	ct vals[4] = { (ct)0, (ct)1, (ct)(mn), (ct)(mx) }; \
	int i, o; \
	for (i = 0; i < 4; i++) { \
		for (o = 0; o < 6; o++) { \
			memory_order ord = pcat_orders[o]; \
			ct before = vals[(i + 1) % 4]; \
			atomic_##tag obj = before; \
			atomic_##tag *volatile p = &obj; \
			ct old = atomic_exchange_explicit(p, vals[i], ord); \
			ct now = *p; \
			pc_check(old == before, "atomic_" #tag " order %s: returned %llx, the previous value was %llx", \
			    pcat_oname(ord), PCAT_V(old), PCAT_V(before)); \
			pc_check(now == vals[i], "atomic_" #tag " order %s: object holds %llx, expected %llx", \
			    pcat_oname(ord), PCAT_V(now), PCAT_V(vals[i])); \
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
