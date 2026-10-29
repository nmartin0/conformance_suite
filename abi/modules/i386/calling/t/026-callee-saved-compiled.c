/*
 * EBX, ESI, EDI and EBP are preserved by compiled code under register pressure.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.calling.026
 */
#include "../../../../check.h"

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.calling.026")
#else
#ifdef __GNUC__
#define ABI_OPT __attribute__((optimize("O2")))
#else
#define ABI_OPT
#endif

/* An external-looking function: the compiler cannot see into it and must
 * keep every live value somewhere that survives the call. */
__attribute__((noinline)) static int opaque(int x)
{
	return x * 3 + 1;
}
static int (*volatile opaque_fp)(int) = opaque;

/* Many values live across calls, so a compiler that is allowed to use
 * EBX, ESI, EDI and EBP as it likes will want all of them. */
ABI_OPT __attribute__((noinline)) int pressure(int a, int b, int c, int d, int e, int f)
{
	int v0 = a + 1, v1 = b + 2, v2 = c + 3, v3 = d + 4, v4 = e + 5, v5 = f + 6;
	int v6 = a ^ b, v7 = c ^ d, v8 = e ^ f, v9 = a * 7;
	int i, sum = 0;

	for (i = 0; i < 6; i++) {
		int r = opaque_fp(i + v0);

		v0 += r + v1;
		v1 += r ^ v2;
		v2 += r - v3;
		v3 += r + v4;
		v4 += r ^ v5;
		v5 += r - v6;
		v6 += r + v7;
		v7 += r ^ v8;
		v8 += r - v9;
		v9 += r + v0;
		sum += v0 ^ v1 ^ v2 ^ v3 ^ v4 ^ v5 ^ v6 ^ v7 ^ v8 ^ v9;
	}
	return sum + v0 + v1 + v2 + v3 + v4 + v5 + v6 + v7 + v8 + v9;
}

int main(void)
{
	pc_begin("ABI.i386.calling.026");
	struct abi_call c;

	pc_test();
	abi_call_init(&c, (void *)(size_t)&pressure);
	abi_call_arg(&c, 1);
	abi_call_arg(&c, 2);
	abi_call_arg(&c, 3);
	abi_call_arg(&c, 4);
	abi_call_arg(&c, 5);
	abi_call_arg(&c, 6);
	abi_call(&c);
	abi_expect_callee_saved(&c, "a compiled function under register pressure");
	pc_pass();
}
#endif
