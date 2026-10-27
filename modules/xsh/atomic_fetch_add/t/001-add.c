/*
 * atomic_fetch_add() adds the operand to the object and returns the value
 * held before, for every atomic integer type.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_fetch_add.001
 */
#define PCAT_ID "XSH.atomic_fetch_add.001"
#include "pcat.h"
#ifndef PCAT_NONE

#define CASE(tag, ct, mn, mx) \
static void run_##tag(void) \
{ \
	atomic_##tag obj = (ct)20; \
	atomic_##tag *volatile p = &obj; \
	ct old, now; \
	old = atomic_fetch_add(p, (ct)7); \
	now = *p; \
	pc_check(old == (ct)20, "atomic_" #tag ": returned %llx, the previous value was 20", PCAT_V(old)); \
	pc_check(now == (ct)27, "atomic_" #tag ": object holds %llx after adding 7 to 20", PCAT_V(now)); \
	old = atomic_fetch_add(p, (ct)0); \
	now = *p; \
	pc_check(old == (ct)27 && now == (ct)27, "atomic_" #tag ": adding 0 gave old %llx, new %llx", \
	    PCAT_V(old), PCAT_V(now)); \
	old = atomic_fetch_add(p, (ct)100); \
	now = *p; \
	pc_check(old == (ct)27 && now == (ct)127, "atomic_" #tag ": adding 100 to 27 gave old %llx, new %llx", \
	    PCAT_V(old), PCAT_V(now)); \
}
PCAT_INTS(CASE)
#undef CASE

int main(void)
{
	pc_begin(PCAT_ID);
	pc_test();
#define CALL(tag, ct, mn, mx) run_##tag();
	PCAT_INTS(CALL)
#undef CALL
	pc_pass();
}
#endif
