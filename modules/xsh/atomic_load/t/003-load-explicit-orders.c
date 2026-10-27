/*
 * atomic_load_explicit() returns the held value with each memory order
 * that is allowed for a load (relaxed, consume, acquire, seq_cst).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_load.003
 */
#define PCAT_ID "XSH.atomic_load.003"
#include "pcat.h"
#ifndef PCAT_NONE

static const memory_order load_orders[4] = {
	memory_order_relaxed, memory_order_consume, memory_order_acquire,
	memory_order_seq_cst
};

#define CASE(tag, ct, mn, mx) \
static void run_##tag(void) \
{ \
	ct vals[4] = { (ct)0, (ct)1, (ct)(mn), (ct)(mx) }; \
	int i, o; \
	for (i = 0; i < 4; i++) { \
		atomic_##tag obj = vals[i]; \
		atomic_##tag *volatile p = &obj; \
		for (o = 0; o < 4; o++) { \
			memory_order ord = load_orders[o]; \
			ct got = atomic_load_explicit(p, ord); \
			pc_check(got == vals[i], "atomic_" #tag " order %s: loaded %llx, object held %llx", \
			    pcat_oname(ord), PCAT_V(got), PCAT_V(vals[i])); \
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
