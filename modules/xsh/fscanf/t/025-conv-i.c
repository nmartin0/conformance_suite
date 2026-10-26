/*
 * %i reads an integer as strtol() with base 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.025
 * MUTANT    fscanf.i_as_d
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
	static const struct { const char *in; int want; } t[] = {
		{ "0x1A", 26 }, { "017", 15 }, { "-0x10", -16 }, { "42", 42 },
	};
	int x, y = -1, r;
	size_t i;

	pc_begin("XSH.fscanf.025");
	pc_test();
	for (i = 0; i < sizeof t / sizeof t[0]; i++) {
		x = -1;
		r = sscanf(t[i].in, "%i", &x);
		pc_check(r == 1 && x == t[i].want, "\"%%i\" on \"%s\" returned %d and %d, expected 1 and %d",
		    t[i].in, r, x, t[i].want);
	}
	x = -1;
	r = sscanf("09", "%i%d", &x, &y);
	pc_check(r == 2 && x == 0 && y == 9, "\"%%i%%d\" on \"09\" returned %d, %d and %d, expected 2, 0 and 9",
	    r, x, y);
	pc_pass();
}
