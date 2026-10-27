/*
 * For signed atomic integer types addition and subtraction wrap around
 * silently on overflow (two's complement), and negative operands work.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_fetch_add.005
 */
#define PCAT_ID "XSH.atomic_fetch_add.005"
#include "pcat.h"
#ifndef PCAT_NONE

static int tested;

#define CASE(tag, ct, mn, mx) \
static void run_##tag(void) \
{ \
	atomic_##tag obj; \
	atomic_##tag *volatile p = &obj; \
	ct max = (ct)(mx), min = (ct)(mn), old, now; \
	if ((ct)-1 > (ct)0) \
		return;		/* an unsigned type: XSH.atomic_fetch_add.004 */ \
	tested++; \
	obj = max; \
	old = atomic_fetch_add(p, (ct)1); \
	now = *p; \
	pc_check(old == max && now == min, "atomic_" #tag ": max + 1 gave old %llx, new %llx (want max, min)", \
	    PCAT_V(old), PCAT_V(now)); \
	old = atomic_fetch_sub(p, (ct)1); \
	now = *p; \
	pc_check(old == min && now == max, "atomic_" #tag ": min - 1 gave old %llx, new %llx (want min, max)", \
	    PCAT_V(old), PCAT_V(now)); \
	old = atomic_fetch_add(p, max); \
	now = *p; \
	pc_check(old == max && now == (ct)-2, "atomic_" #tag ": max + max gave old %llx, new %llx (want max, -2)", \
	    PCAT_V(old), PCAT_V(now)); \
	obj = (ct)5; \
	old = atomic_fetch_add(p, (ct)-8); \
	now = *p; \
	pc_check(old == (ct)5 && now == (ct)-3, "atomic_" #tag ": 5 + -8 gave old %llx, new %llx (want 5, -3)", \
	    PCAT_V(old), PCAT_V(now)); \
	old = atomic_fetch_sub(p, (ct)-4); \
	now = *p; \
	pc_check(old == (ct)-3 && now == (ct)1, "atomic_" #tag ": -3 - -4 gave old %llx, new %llx (want -3, 1)", \
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
		pc_unresolved("only %d signed types found", tested);
	pc_pass();
}
#endif
