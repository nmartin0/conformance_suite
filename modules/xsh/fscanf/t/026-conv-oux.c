/*
 * %o, %u, %x and %X read as strtoul() with bases 8, 10 and 16.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.026
 * MUTANT    fscanf.o_as_u
 * MUTANT    fscanf.upper_unknown
 */
#include "pc.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

int main(void)
{
	static const struct { const char *in, *fmt; unsigned want; } t[] = {
		{ "17", "%o", 15 }, { "42", "%u", 42 }, { "1aF", "%x", 0x1af },
		{ "0X1f", "%x", 0x1f }, { "ff", "%X", 0xff },
	};
	unsigned u;
	int r;
	size_t i;

	pc_begin("XSH.fscanf.026");
	pc_test();
	for (i = 0; i < sizeof t / sizeof t[0]; i++) {
		u = 12345;
		r = sscanf(t[i].in, t[i].fmt, &u);
		pc_check(r == 1 && u == t[i].want, "\"%s\" on \"%s\" returned %d and %u, expected 1 and %u",
		    t[i].fmt, t[i].in, r, u, t[i].want);
	}
	r = sscanf("8", "%o", &u);
	pc_check(r == 0, "\"%%o\" on \"8\" returned %d, expected 0", r);
	pc_pass();
}
