/*
 * %a reads the floating forms; all of them accept hexadecimal input.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.028
 * MUTANT    fscanf.no_hex_float
 * VARIANT   fscanf.float_ulp
 */
#include "pc.h"
#include <errno.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/*
 * Within a relative 4*EPSILON of want (4 to 8 ulps, by where want sits in
 * its binade). ISO C makes the accuracy of string-to-floating conversion
 * implementation-defined and the text sets no bound: this is a practical
 * cap, not a requirement.
 */
static int near_f(float x, float want)
{
	float d = x > want ? x - want : want - x;

	return d <= 4 * FLT_EPSILON * (want < 0 ? -want : want);
}

int main(void)
{
	static const struct { const char *in, *fmt; float want; } t[] = {
		{ "0x1.8p1", "%a", 3.0f }, { "0x1p-2", "%f", 0.25f }, { "1.5", "%a", 1.5f },
	};
	float x;
	int r;
	size_t i;

	pc_begin("XSH.fscanf.028");
	pc_test();
	for (i = 0; i < sizeof t / sizeof t[0]; i++) {
		x = -1;
		r = sscanf(t[i].in, t[i].fmt, &x);
		pc_check(r == 1 && near_f(x, t[i].want), "\"%s\" on \"%s\" returned %d and %g, expected 1 and %g",
		    t[i].fmt, t[i].in, r, (double)x, (double)t[i].want);
	}
	pc_pass();
}
