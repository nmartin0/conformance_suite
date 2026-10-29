/*
 * At function entry ESP + 4 is a multiple of 16 when compiled code makes the call.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.calling.011
 */
#include "../../../../check.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.calling.011")
#else
ABI_TARGET void call_body(void)
{
}

void call_tramp(void);
ABI_ENTRY_TRAMPOLINE(call_tramp, call_body);
static void (*volatile fp)(void) = call_tramp;

struct big { int a[5]; };
struct odd { char c[7]; };

/* A frame of every size from 1 to 40 bytes, then a call with one argument. */
static void with_frame(int n)
{
	volatile char v[n];

	v[0] = 1;
	v[n - 1] = 2;
	((void (*)(int))fp)(n);
}
int main(void)
{
	pc_begin("ABI.i386.calling.011");
	struct big b = { { 1, 2, 3, 4, 5 } };
	struct odd o = { { 1, 2, 3, 4, 5, 6, 7 } };
	int n;

	pc_test();
	((void (*)(void))fp)();
	((void (*)(int))fp)(1);
	((void (*)(int, int))fp)(1, 2);
	((void (*)(int, int, int))fp)(1, 2, 3);
	((void (*)(int, int, int, int))fp)(1, 2, 3, 4);
	((void (*)(int, int, int, int, int))fp)(1, 2, 3, 4, 5);
	((void (*)(int, int, int, int, int, int, int))fp)(1, 2, 3, 4, 5, 6, 7);
	((void (*)(double))fp)(1.0);
	((void (*)(int, double, char))fp)(1, 2.0, 'c');
	((void (*)(long double))fp)(1.0L);
	((void (*)(long long, int))fp)(1LL, 2);
	((void (*)(struct big))fp)(b);
	((void (*)(char, struct odd, short))fp)('x', o, 3);
	for (n = 1; n <= 40; n++)
		with_frame(n);
	abi_expect_entry_mod16(&call_tramp_rec, "a function called from compiled code");
	pc_pass();
}
#endif
