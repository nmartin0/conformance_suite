/*
 * For unsigned atomic integer types addition and subtraction wrap around
 * modulo the type's range.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_fetch_add.004
 */
#define PCAT_ID "XSH.atomic_fetch_add.004"
#include "pcat.h"
#ifndef PCAT_NONE

static int tested;

#define CASE(tag, ct, mn, mx) \
static void run_##tag(void) \
{ \
	atomic_##tag obj; \
	atomic_##tag *volatile p = &obj; \
	ct max = (ct)(mx), old, now; \
	if (!((ct)-1 > (ct)0)) \
		return;		/* a signed type: XSH.atomic_fetch_add.005 */ \
	tested++; \
	obj = max; \
	old = atomic_fetch_add(p, (ct)1); \
	now = *p; \
	pc_check(old == max && now == (ct)0, "atomic_" #tag ": max + 1 gave old %llx, new %llx (want max, 0)", \
	    PCAT_V(old), PCAT_V(now)); \
	old = atomic_fetch_sub(p, (ct)1); \
	now = *p; \
	pc_check(old == (ct)0 && now == max, "atomic_" #tag ": 0 - 1 gave old %llx, new %llx (want 0, max)", \
	    PCAT_V(old), PCAT_V(now)); \
	old = atomic_fetch_add(p, max); \
	now = *p; \
	pc_check(old == max && now == (ct)(max - 1), "atomic_" #tag ": max + max gave old %llx, new %llx (want max, max-1)", \
	    PCAT_V(old), PCAT_V(now)); \
	obj = (ct)1; \
	old = atomic_fetch_sub(p, (ct)2); \
	now = *p; \
	pc_check(old == (ct)1 && now == max, "atomic_" #tag ": 1 - 2 gave old %llx, new %llx (want 1, max)", \
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
	if (tested < 4)
		pc_unresolved("only %d unsigned types found", tested);
	pc_pass();
}
#endif
