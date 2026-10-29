/*
 * Structures of 12 to 100 bytes are returned through a hidden pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.calling.015
 */
#include "../../../../check.h"
#include <string.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.calling.015")
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

struct g1 { int a, b, c; };
struct g2 { double d[2]; };
struct g3 { int a[25]; };
struct g4 { long long x; int y; };
struct g5 { char c[33]; };
struct g6 { long double x; };
DEF(struct g1, f_g1, { 1, 2, 3 })
DEF(struct g2, f_g2, { { 1.5, 2.5 } })
DEF(struct g3, f_g3, { { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 } })
DEF(struct g4, f_g4, { 0x0123456789ABCDEFLL, 77 })
DEF(struct g5, f_g5, { { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33 } })
DEF(struct g6, f_g6, { 3.25L })
int main(void)
{
	pc_begin("ABI.i386.calling.015");
	pc_test();
	AGG(struct g1, f_g1, { 1, 2, 3 });
	AGG(struct g2, f_g2, { { 1.5, 2.5 } });
	AGG(struct g3, f_g3, { { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 } });
	AGG(struct g4, f_g4, { 0x0123456789ABCDEFLL, 77 });
	AGG(struct g5, f_g5, { { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33 } });
	AGGEQ(struct g6, f_g6, box.v.x == want.x, { 3.25L });
	pc_pass();
}
#endif
