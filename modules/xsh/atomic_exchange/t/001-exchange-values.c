/*
 * atomic_exchange() installs the new value and returns the previous one,
 * for every atomic integer type.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_exchange.001
 */
#define PCAT_ID "XSH.atomic_exchange.001"
#include "pcat.h"
#ifndef PCAT_NONE

#define CASE(tag, ct, mn, mx) \
static void run_##tag(void) \
{ \
	ct vals[4] = { (ct)0, (ct)1, (ct)(mn), (ct)(mx) }; \
	int i; \
	for (i = 0; i < 4; i++) { \
		ct before = vals[(i + 1) % 4]; \
		atomic_##tag obj = before; \
		atomic_##tag *volatile p = &obj; \
		ct old = atomic_exchange(p, vals[i]); \
		ct now = *p; \
		pc_check(old == before, "atomic_" #tag ": returned %llx, the previous value was %llx", \
		    PCAT_V(old), PCAT_V(before)); \
		pc_check(now == vals[i], "atomic_" #tag ": object holds %llx, expected %llx", \
		    PCAT_V(now), PCAT_V(vals[i])); \
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
