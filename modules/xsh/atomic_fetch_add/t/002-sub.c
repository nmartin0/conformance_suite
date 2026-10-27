/*
 * atomic_fetch_sub() subtracts the operand from the object and returns
 * the value held before, for every atomic integer type.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_fetch_add.002
 */
#define PCAT_ID "XSH.atomic_fetch_add.002"
#include "pcat.h"
#ifndef PCAT_NONE

#define CASE(tag, ct, mn, mx) \
static void run_##tag(void) \
{ \
	atomic_##tag obj = (ct)127; \
	atomic_##tag *volatile p = &obj; \
	ct old, now; \
	old = atomic_fetch_sub(p, (ct)7); \
	now = *p; \
	pc_check(old == (ct)127, "atomic_" #tag ": returned %llx, the previous value was 127", PCAT_V(old)); \
	pc_check(now == (ct)120, "atomic_" #tag ": object holds %llx after subtracting 7 from 127", PCAT_V(now)); \
	old = atomic_fetch_sub(p, (ct)0); \
	now = *p; \
	pc_check(old == (ct)120 && now == (ct)120, "atomic_" #tag ": subtracting 0 gave old %llx, new %llx", \
	    PCAT_V(old), PCAT_V(now)); \
	old = atomic_fetch_sub(p, (ct)120); \
	now = *p; \
	pc_check(old == (ct)120 && now == (ct)0, "atomic_" #tag ": subtracting 120 from 120 gave old %llx, new %llx", \
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
