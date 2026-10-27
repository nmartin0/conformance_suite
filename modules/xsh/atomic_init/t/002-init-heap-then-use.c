/*
 * An atomic object in memory that held junk, once initialized with
 * atomic_init(), works for every operation: load, fetch_add, exchange,
 * compare-and-exchange and store, for every atomic integer type.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_init.002
 */
#define PCAT_ID "XSH.atomic_init.002"
#include "pcat.h"
#ifndef PCAT_NONE

#define CASE(tag, ct, mn, mx) \
static void run_##tag(void) \
{ \
	atomic_##tag *volatile p = malloc(sizeof(atomic_##tag)); \
	ct old, exp; \
	_Bool r; \
	if (p == NULL) \
		pc_unresolved("malloc failed"); \
	memset((void *)p, 0x5a, sizeof(atomic_##tag)); \
	atomic_init(p, (ct)20); \
	pc_check(atomic_load(p) == (ct)20, "atomic_" #tag ": not 20 after atomic_init"); \
	old = atomic_fetch_add(p, (ct)5); \
	pc_check(old == (ct)20 && atomic_load(p) == (ct)25, "atomic_" #tag ": fetch_add gave old %llx", PCAT_V(old)); \
	old = atomic_exchange(p, (ct)9); \
	pc_check(old == (ct)25 && atomic_load(p) == (ct)9, "atomic_" #tag ": exchange gave old %llx", PCAT_V(old)); \
	exp = 9; \
	r = atomic_compare_exchange_strong(p, &exp, (ct)30); \
	pc_check(r && atomic_load(p) == (ct)30, "atomic_" #tag ": compare-and-exchange failed"); \
	atomic_store(p, (ct)12); \
	pc_check(atomic_load(p) == (ct)12, "atomic_" #tag ": store/load gave the wrong value"); \
	free((void *)p); \
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
