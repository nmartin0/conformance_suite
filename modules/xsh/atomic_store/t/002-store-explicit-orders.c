/*
 * atomic_store_explicit() replaces the value with each memory order that
 * is allowed for a store (relaxed, release, seq_cst).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_store.002
 */
#define PCAT_ID "XSH.atomic_store.002"
#include "pcat.h"
#ifndef PCAT_NONE

static const memory_order store_orders[3] = {
	memory_order_relaxed, memory_order_release, memory_order_seq_cst
};

#define CASE(tag, ct, mn, mx) \
static void run_##tag(void) \
{ \
	ct vals[4] = { (ct)0, (ct)1, (ct)(mn), (ct)(mx) }; \
	int i, o; \
	for (i = 0; i < 4; i++) { \
		for (o = 0; o < 3; o++) { \
			memory_order ord = store_orders[o]; \
			atomic_##tag obj = vals[(i + 1) % 4]; \
			atomic_##tag *volatile p = &obj; \
			ct got; \
			atomic_store_explicit(p, vals[i], ord); \
			got = *p; \
			pc_check(got == vals[i], "atomic_" #tag " order %s: object holds %llx after storing %llx", \
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
