/*
 * offsetof gives the right offsets and is a constant expression.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stddef.h.002
 */
#include <stddef.h>
#include "pc.h"

struct in { char a; long b; };
struct s { char a; short b; int c; double d; char e[3]; struct in in; char tail; };

_Static_assert(_Generic(offsetof(struct s, c), size_t: 1, default: 0), "offsetof does not have type size_t");
_Static_assert(offsetof(struct s, a) == 0, "the first member is not at offset 0");
_Static_assert(offsetof(struct s, b) >= offsetof(struct s, a) + sizeof(char), "b overlaps a");
_Static_assert(offsetof(struct s, c) >= offsetof(struct s, b) + sizeof(short), "c overlaps b");
_Static_assert(offsetof(struct s, d) >= offsetof(struct s, c) + sizeof(int), "d overlaps c");
_Static_assert(offsetof(struct s, e) >= offsetof(struct s, d) + sizeof(double), "e overlaps d");
_Static_assert(offsetof(struct s, in) >= offsetof(struct s, e) + 3, "in overlaps e");
_Static_assert(offsetof(struct s, tail) >= offsetof(struct s, in) + sizeof(struct in), "tail overlaps in");
_Static_assert(offsetof(struct s, tail) + sizeof(char) <= sizeof(struct s), "tail does not fit in the structure");
_Static_assert(offsetof(struct s, b) % _Alignof(short) == 0 && offsetof(struct s, c) % _Alignof(int) == 0 &&
    offsetof(struct s, d) % _Alignof(double) == 0 && offsetof(struct s, in) % _Alignof(struct in) == 0, "a member offset is not suitably aligned");
_Static_assert(offsetof(struct s, in.b) == offsetof(struct s, in) + offsetof(struct in, b), "a nested designator gives the wrong offset");
_Static_assert(offsetof(struct s, e[2]) == offsetof(struct s, e) + 2, "an array-element designator gives the wrong offset");

static char sized[offsetof(struct s, c) + 1];

int main(void)
{
	pc_begin("XBD.stddef.h.002");
	pc_test();
	static struct s x;
	char *base = (char *)&x;

	pc_check(sizeof sized > 0, "unreachable");
	pc_check((char *)&x.b - base == (ptrdiff_t)offsetof(struct s, b), "offsetof(b) differs from the real offset");
	pc_check((char *)&x.c - base == (ptrdiff_t)offsetof(struct s, c), "offsetof(c) differs from the real offset");
	pc_check((char *)&x.d - base == (ptrdiff_t)offsetof(struct s, d), "offsetof(d) differs from the real offset");
	pc_check((char *)&x.e[2] - base == (ptrdiff_t)offsetof(struct s, e[2]), "offsetof(e[2]) differs from the real offset");
	pc_check((char *)&x.in.b - base == (ptrdiff_t)offsetof(struct s, in.b), "offsetof(in.b) differs from the real offset");
	pc_check((char *)&x.tail - base == (ptrdiff_t)offsetof(struct s, tail), "offsetof(tail) differs from the real offset");
	pc_pass();
}
