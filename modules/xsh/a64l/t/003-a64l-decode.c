/*
 * a64l: a64l decode.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.a64l.003
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>
static long (*volatile f_a64l)(const char *) = a64l;
static char *(*volatile f_l64a)(long) = l64a;
static const char alpha[] = "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

int main(void)
{
	static const struct { const char *s; long v; } t[] = {
		{ ".", 0 }, { "/", 1 }, { "0", 2 }, { "9", 11 }, { "A", 12 }, { "Z", 37 }, { "a", 38 }, { "z", 63 },
		{ "./", 64 }, { "/.", 1 }, { "zzzzz/", 2147483647L }, { "..", 0 }, { "aA", 38 + 12 * 64 }, { "", 0 }, { "//", 65 }, { "../", 4096 }
	};
	size_t k;
	pc_begin("XSH.a64l.003");
	pc_test();
	for (k = 0; k < sizeof t / sizeof t[0]; k++)
		pc_check(f_a64l(t[k].s) == t[k].v, "a64l(\"%s\") was %ld, expected %ld", t[k].s, f_a64l(t[k].s), t[k].v);
	pc_pass();
}
