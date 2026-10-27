/*
 * atomic_load() accepts a pointer to a const and/or volatile qualified
 * atomic object, returns its value, and leaves the object unchanged.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_load.004
 */
#define PCAT_ID "XSH.atomic_load.004"
#include "pcat.h"
#ifndef PCAT_NONE

static const atomic_int cobj = 31;

int main(void)
{
	atomic_int obj = 77;
	atomic_ullong big = 0x123456789abcdefULL;
	atomic_int *volatile q = &obj;
	atomic_ullong *volatile qb = &big;
	const volatile atomic_int *volatile cvp = &obj;
	const atomic_int *volatile cp = &cobj;
	const volatile atomic_ullong *volatile cvb = &big;
	int a, b, c, d;
	unsigned long long e;

	pc_begin(PCAT_ID);
	pc_test();
	a = atomic_load(cvp);
	b = atomic_load_explicit(cvp, memory_order_relaxed);
	c = atomic_load(cp);
	d = atomic_load_explicit(cp, memory_order_acquire);
	e = atomic_load(cvb);
	pc_check(a == 77, "load through const volatile pointer gave %d, not 77", a);
	pc_check(b == 77, "explicit load through const volatile pointer gave %d, not 77", b);
	pc_check(c == 31, "load of a const atomic object gave %d, not 31", c);
	pc_check(d == 31, "explicit load of a const atomic object gave %d, not 31", d);
	pc_check(e == 0x123456789abcdefULL, "wide load gave %llx", e);
	/* Loading changes nothing: the object, read plainly, still holds its value. */
	a = *q;
	pc_check(a == 77, "the object holds %d after loads, not 77", a);
	e = *qb;
	pc_check(e == 0x123456789abcdefULL, "the wide object holds %llx after loads", e);
	pc_pass();
}
#endif
