/*
 * atomic_fetch_and(), atomic_fetch_or() and atomic_fetch_xor() apply the
 * bitwise operation to the object and return the value held before, for
 * every atomic integer type.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_fetch_add.003
 */
#define PCAT_ID "XSH.atomic_fetch_add.003"
#include "pcat.h"
#ifndef PCAT_NONE

#define CASE(tag, ct, mn, mx) \
static void run_##tag(void) \
{ \
	atomic_##tag a = (ct)0x55, o = (ct)0x55, x = (ct)0x55, z = (ct)0x2a; \
	atomic_##tag *volatile pa = &a, *volatile po = &o, *volatile px = &x, *volatile pz = &z; \
	ct old, now; \
	old = atomic_fetch_and(pa, (ct)0x33); \
	now = *pa; \
	pc_check(old == (ct)0x55 && now == (ct)0x11, "atomic_" #tag " and: old %llx, new %llx (want 55, 11)", \
	    PCAT_V(old), PCAT_V(now)); \
	old = atomic_fetch_or(po, (ct)0x33); \
	now = *po; \
	pc_check(old == (ct)0x55 && now == (ct)0x77, "atomic_" #tag " or: old %llx, new %llx (want 55, 77)", \
	    PCAT_V(old), PCAT_V(now)); \
	old = atomic_fetch_xor(px, (ct)0x33); \
	now = *px; \
	pc_check(old == (ct)0x55 && now == (ct)0x66, "atomic_" #tag " xor: old %llx, new %llx (want 55, 66)", \
	    PCAT_V(old), PCAT_V(now)); \
	old = atomic_fetch_xor(px, (ct)0x33); \
	now = *px; \
	pc_check(old == (ct)0x66 && now == (ct)0x55, "atomic_" #tag " xor again: old %llx, new %llx (want 66, 55)", \
	    PCAT_V(old), PCAT_V(now)); \
	old = atomic_fetch_and(pz, (ct)0x55); \
	now = *pz; \
	pc_check(old == (ct)0x2a && now == (ct)0, "atomic_" #tag " and to zero: old %llx, new %llx (want 2a, 0)", \
	    PCAT_V(old), PCAT_V(now)); \
	old = atomic_fetch_or(pz, (ct)0x2a); \
	now = *pz; \
	pc_check(old == (ct)0 && now == (ct)0x2a, "atomic_" #tag " or from zero: old %llx, new %llx (want 0, 2a)", \
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
