/*
 * The _explicit forms of atomic_fetch_add/sub/and/or/xor give the same
 * results as the plain forms with every memory order, for every atomic
 * integer type.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_fetch_add.007
 */
#define PCAT_ID "XSH.atomic_fetch_add.007"
#include "pcat.h"
#ifndef PCAT_NONE

#define CHECK(tag, name, ord, old, want_old, now, want_now) \
	pc_check((old) == (want_old) && (now) == (want_now), \
	    "atomic_" #tag " %s order %s: old %llx, new %llx (want %llx, %llx)", name, pcat_oname(ord), \
	    PCAT_V(old), PCAT_V(now), PCAT_V(want_old), PCAT_V(want_now))

#define CASE(tag, ct, mn, mx) \
static void run_##tag(void) \
{ \
	int o; \
	for (o = 0; o < 6; o++) { \
		memory_order ord = pcat_orders[o]; \
		atomic_##tag a = (ct)20, s = (ct)60, n = (ct)0x55, r = (ct)0x55, x = (ct)0x55; \
		atomic_##tag *volatile pa = &a, *volatile ps = &s, *volatile pn = &n, *volatile pr = &r, \
		    *volatile px = &x; \
		ct old, now; \
		old = atomic_fetch_add_explicit(pa, (ct)7, ord); \
		now = *pa; \
		CHECK(tag, "fetch_add", ord, old, (ct)20, now, (ct)27); \
		old = atomic_fetch_sub_explicit(ps, (ct)7, ord); \
		now = *ps; \
		CHECK(tag, "fetch_sub", ord, old, (ct)60, now, (ct)53); \
		old = atomic_fetch_and_explicit(pn, (ct)0x33, ord); \
		now = *pn; \
		CHECK(tag, "fetch_and", ord, old, (ct)0x55, now, (ct)0x11); \
		old = atomic_fetch_or_explicit(pr, (ct)0x33, ord); \
		now = *pr; \
		CHECK(tag, "fetch_or", ord, old, (ct)0x55, now, (ct)0x77); \
		old = atomic_fetch_xor_explicit(px, (ct)0x33, ord); \
		now = *px; \
		CHECK(tag, "fetch_xor", ord, old, (ct)0x55, now, (ct)0x66); \
	} \
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
