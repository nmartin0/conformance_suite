/*
 * atomic_is_lock_free() gives the same answer every time and for every
 * pointer of the same type: a second object, a qualified pointer, and a
 * null pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_is_lock_free.001
 */
#define PCAT_ID "XSH.atomic_is_lock_free.001"
#include "pcat.h"
#ifndef PCAT_NONE

struct rec { int a; };

#define CASE(tag, ct, mn, mx) \
static void run_##tag(void) \
{ \
	static atomic_##tag first, second; \
	atomic_##tag third; \
	const volatile atomic_##tag *volatile q = &second; \
	atomic_##tag *volatile np = NULL; \
	int a = !!atomic_is_lock_free(&first); \
	int b = !!atomic_is_lock_free(&first); \
	int c = !!atomic_is_lock_free(&second); \
	int d = !!atomic_is_lock_free(&third); \
	int e = !!atomic_is_lock_free(q); \
	int f = !!atomic_is_lock_free(np); \
	pc_check(a == b && a == c && a == d && a == e && a == f, \
	    "atomic_" #tag ": answers differ (%d %d %d %d %d %d)", a, b, c, d, e, f); \
}
PCAT_ALL(CASE)
#undef CASE

#define PCASE(name, T) \
static void run_##name(void) \
{ \
	static _Atomic(T *) first, second; \
	_Atomic(T *) third; \
	_Atomic(T *) *volatile np = NULL; \
	int a = !!atomic_is_lock_free(&first); \
	int b = !!atomic_is_lock_free(&first); \
	int c = !!atomic_is_lock_free(&second); \
	int d = !!atomic_is_lock_free(&third); \
	int f = !!atomic_is_lock_free(np); \
	pc_check(a == b && a == c && a == d && a == f, \
	    "pointer to " #name ": answers differ (%d %d %d %d %d)", a, b, c, d, f); \
}
PCASE(vd, void)
PCASE(ch, char)
PCASE(in, int)
PCASE(rc, struct rec)

int main(void)
{
	pc_begin(PCAT_ID);
	pc_test();
#define CALL(tag, ct, mn, mx) run_##tag();
	PCAT_ALL(CALL)
#undef CALL
	run_vd();
	run_ch();
	run_in();
	run_rc();
	pc_pass();
}
#endif
