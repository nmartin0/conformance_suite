/*
 * A structure member goes to the lowest offset that suits its alignment.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ABI.i386.types.010
 */
#include "../../../../check.h"
#include <stddef.h>

#ifndef ABI_ARCH_I386
ABI_NOT_THIS_ARCH("ABI.i386.types.010")
#else
struct p1 { char c; int i; };
struct p2 { char c; short s; };
struct p3 { char a; char b; short c; int d; };
struct p4 { char a[3]; char c; };
struct p5 { char c; double d; char e; };
struct p6 { char c; short s; int i; };
int main(void)
{
	pc_begin("ABI.i386.types.010");
	pc_test();
	_Static_assert(offsetof(struct p1, i) == 4, "int after char: offset 4");
	_Static_assert(offsetof(struct p2, s) == 2, "short after char: offset 2");
	_Static_assert(offsetof(struct p3, b) == 1 && offsetof(struct p3, c) == 2 && offsetof(struct p3, d) == 4,
	    "char, char, short, int must be at 0, 1, 2, 4");
	_Static_assert(offsetof(struct p4, c) == 3, "a char array of 3 then a char: offsets 0 and 3");
	_Static_assert(offsetof(struct p5, d) == 4 && offsetof(struct p5, e) == 12, "char, double, char: offsets 0, 4, 12");
	_Static_assert(offsetof(struct p6, s) == 2 && offsetof(struct p6, i) == 4, "char, short, int: offsets 0, 2, 4");
	pc_pass();
}
#endif
