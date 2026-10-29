/*
 * _Complex double and _Complex long double are returned through a hidden pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.calling.016
 */
#include "../../../../check.h"
#include <string.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.calling.016")
#else
static const unsigned char guard_byte = 0xA5;

static void check_guard(const unsigned char *p, size_t n, const char *what)
{
	size_t i;

	for (i = 0; i < n; i++)
		pc_check(p[i] == guard_byte, "%s wrote outside the returned object (byte %lu of the guard)", what, (unsigned long)i);
}

/* Call fn(hidden pointer), check the hidden-pointer protocol and that the object holds want (size n). */
#define AGGEQ(T, fn, EQ, ...)                                                             \
	do {                                                                           \
		struct { unsigned char pre[16]; T v; unsigned char post[16]; } box;    \
		T want = __VA_ARGS__;                                                      \
		struct abi_call c;                                                     \
		memset(&box, guard_byte, sizeof box);                                  \
		abi_call_init(&c, (void *)(size_t)&fn);                                \
		abi_call_arg(&c, (unsigned)(size_t)&box.v);                            \
		abi_call(&c);                                                          \
		abi_expect_hidden_pointer(&c, &box.v, #fn);                            \
		pc_check(EQ,                                                           \
		    "%s did not write its value through the hidden pointer", #fn);     \
		check_guard(box.pre, sizeof box.pre, #fn);                             \
		check_guard(box.post, sizeof box.post, #fn);                           \
	} while (0)

#define AGG(T, fn, ...) AGGEQ(T, fn, memcmp(&box.v, &want, sizeof want) == 0, __VA_ARGS__)
#define DEF(T, fn, ...) __attribute__((noinline)) T fn(void) { T r = __VA_ARGS__; return r; }

/* a complex number from its two parts (CMPLX is not in every compiler's headers) */
static _Complex double mk_cd(double r, double i)
{
	double p[2];
	_Complex double z;

	p[0] = r;
	p[1] = i;
	memcpy(&z, p, sizeof z);
	return z;
}

static _Complex long double mk_cl(long double r, long double i)
{
	long double p[2];
	_Complex long double z;

	p[0] = r;
	p[1] = i;
	memcpy(&z, p, sizeof z);
	return z;
}

__attribute__((noinline)) _Complex double f_cd(void) { return mk_cd(1.5, 2.5); }
__attribute__((noinline)) _Complex long double f_cl(void) { return mk_cl(1.5L, 2.5L); }
int main(void)
{
	pc_begin("ABI.i386.calling.016");
	pc_test();
	{
		_Complex double want = mk_cd(1.5, 2.5);
		struct { unsigned char pre[16]; _Complex double v; unsigned char post[16]; } box;
		struct abi_call c;

		memset(&box, guard_byte, sizeof box);
		abi_call_init(&c, (void *)(size_t)&f_cd);
		abi_call_arg(&c, (unsigned)(size_t)&box.v);
		abi_call(&c);
		abi_expect_hidden_pointer(&c, &box.v, "f_cd");
		pc_check(box.v == want, "f_cd did not write its value through the hidden pointer");
		check_guard(box.pre, sizeof box.pre, "f_cd");
		check_guard(box.post, sizeof box.post, "f_cd");
	}
	{
		_Complex long double want = mk_cl(1.5L, 2.5L);
		struct { unsigned char pre[16]; _Complex long double v; unsigned char post[16]; } box;
		struct abi_call c;

		memset(&box, guard_byte, sizeof box);
		abi_call_init(&c, (void *)(size_t)&f_cl);
		abi_call_arg(&c, (unsigned)(size_t)&box.v);
		abi_call(&c);
		abi_expect_hidden_pointer(&c, &box.v, "f_cl");
		pc_check(box.v == want, "f_cl did not write its value through the hidden pointer");
		check_guard(box.pre, sizeof box.pre, "f_cl");
		check_guard(box.post, sizeof box.post, "f_cl");
	}
	pc_pass();
}
#endif
