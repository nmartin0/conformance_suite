/*
 * libcalls.h - standard-library calls made with the ABI's sentinels loaded.
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Four calls whose effect on the machine state the calling-sequence tests
 * examine: strlen, memcpy, qsort (which calls back into C) and snprintf
 * with floating-point arguments. abi_lib_run() makes one of them through
 * abi_call_raw() and checks that it still did its job; the tests then look
 * at the registers and control words the record holds. Generic C: the only
 * architecture-specific part is struct abi_call.
 */
#ifndef ABI_LIBCALLS_H
#define ABI_LIBCALLS_H

#include "check.h"

#ifdef ABI_ARCH_I386

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { ABI_LIB_STRLEN, ABI_LIB_MEMCPY, ABI_LIB_QSORT, ABI_LIB_SNPRINTF, ABI_LIB_N };

static const char *const abi_lib_name[ABI_LIB_N] = { "strlen", "memcpy", "qsort", "snprintf" };

static int abi_lib_cmp(const void *a, const void *b)
{
	int x = *(const int *)a, y = *(const int *)b;

	return (x > y) - (x < y);
}

/*
 * Make library call k. With altfp the x87 control word and MXCSR are set to
 * non-default values just before it, so that a library that changed them
 * and did not put them back is seen. A call that does not do its job is a
 * failure of that call.
 */
static void abi_lib_run(int k, struct abi_call *c, int altfp)
{
	static const char text[] = "twelve bytes";
	static char dst[32];
	static int arr[8] = { 5, 3, 9, 1, 7, 2, 8, 4 };
	static char out[64];
	static const int sorted[8] = { 1, 2, 3, 4, 5, 7, 8, 9 };

	switch (k) {
	case ABI_LIB_STRLEN:
		abi_call_init(c, (void *)(size_t)&strlen);
		abi_call_arg(c, (unsigned)(size_t)text);
		break;
	case ABI_LIB_MEMCPY:
		memset(dst, 0, sizeof dst);
		abi_call_init(c, (void *)(size_t)&memcpy);
		abi_call_arg(c, (unsigned)(size_t)dst);
		abi_call_arg(c, (unsigned)(size_t)text);
		abi_call_arg(c, (unsigned)sizeof text);
		break;
	case ABI_LIB_QSORT:
		abi_call_init(c, (void *)(size_t)&qsort);
		abi_call_arg(c, (unsigned)(size_t)arr);
		abi_call_arg(c, 8);
		abi_call_arg(c, (unsigned)sizeof arr[0]);
		abi_call_arg(c, (unsigned)(size_t)abi_lib_cmp);
		break;
	default:
		memset(out, 0, sizeof out);
		abi_call_init(c, (void *)(size_t)&snprintf);
		abi_call_arg(c, (unsigned)(size_t)out);
		abi_call_arg(c, (unsigned)sizeof out);
		abi_call_arg(c, (unsigned)(size_t)"%.1f %.2f %.3Lf");
		abi_call_arg_double(c, 1.5);
		abi_call_arg_double(c, (double)2.25f);
		abi_call_arg_ldouble(c, 3.125L);
		break;
	}
	if (altfp) {
		c->flags |= ABI_CALL_SETFP;
		c->cw_set = ABI_X87_CW_ALT;
		c->mxcsr_set = ABI_MXCSR_ALT;
	}
	abi_call(c);
	abi_require_x87_empty(c);
	if (altfp) {
		if (c->cw_before != ABI_X87_CW_ALT)
			pc_unresolved("could not load the x87 control word 0x%04x (it reads 0x%04x)",
			    ABI_X87_CW_ALT, c->cw_before);
		if (!(c->flags & ABI_CALL_NOSSE) &&
		    (c->mxcsr_before & ABI_MXCSR_CONTROL) != (ABI_MXCSR_ALT & ABI_MXCSR_CONTROL))
			pc_unresolved("could not load MXCSR 0x%08x (it reads 0x%08x)", ABI_MXCSR_ALT,
			    c->mxcsr_before);
	}
	switch (k) {
	case ABI_LIB_STRLEN:
		pc_check(c->eax == 12, "strlen returned %u for a 12-byte string", c->eax);
		break;
	case ABI_LIB_MEMCPY:
		pc_check(c->eax == (unsigned)(size_t)dst, "memcpy returned 0x%08x, not its destination", c->eax);
		pc_check(memcmp(dst, text, sizeof text) == 0, "memcpy did not copy the bytes");
		break;
	case ABI_LIB_QSORT:
		pc_check(memcmp(arr, sorted, sizeof arr) == 0, "qsort did not sort the array");
		/* put it back out of order for another run */
		arr[0] = 5; arr[1] = 3; arr[2] = 9; arr[3] = 1; arr[4] = 7; arr[5] = 2; arr[6] = 8; arr[7] = 4;
		break;
	default:
		pc_check(strcmp(out, "1.5 2.25 3.125") == 0, "snprintf wrote \"%s\", expected \"1.5 2.25 3.125\"", out);
		break;
	}
}

#endif /* ABI_ARCH_I386 */
#endif /* ABI_LIBCALLS_H */
