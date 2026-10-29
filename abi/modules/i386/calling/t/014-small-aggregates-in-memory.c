/*
 * Structures of 1, 2, 3, 4 and 8 bytes are returned through a hidden pointer, not in registers.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.calling.014
 */
#include "../../../../check.h"
#include <string.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.calling.014")
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

struct a1 { char c; };
struct a2 { short s; };
struct a3 { char c[3]; };
struct a4 { int i; };
struct a5 { float f; };
struct a6 { int a, b; };
struct a7 { double d; };
DEF(struct a1, f_a1, { 0x5A })
DEF(struct a2, f_a2, { 0x1234 })
DEF(struct a3, f_a3, { { 1, 2, 3 } })
DEF(struct a4, f_a4, { 0x12345678 })
DEF(struct a5, f_a5, { 1.5f })
DEF(struct a6, f_a6, { 7, 9 })
DEF(struct a7, f_a7, { 2.5 })
int main(void)
{
	pc_begin("ABI.i386.calling.014");
	pc_test();
	AGG(struct a1, f_a1, { 0x5A });
	AGG(struct a2, f_a2, { 0x1234 });
	AGG(struct a3, f_a3, { { 1, 2, 3 } });
	AGG(struct a4, f_a4, { 0x12345678 });
	AGG(struct a5, f_a5, { 1.5f });
	AGG(struct a6, f_a6, { 7, 9 });
	AGG(struct a7, f_a7, { 2.5 });
	pc_pass();
}
#endif
