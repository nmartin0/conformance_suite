/*
 * %A, %E, %F, %G and %X convert as their lower-case forms.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.029
 * MUTANT    fscanf.upper_unknown
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
		{ "0x1p1", "%A", 2.0f }, { "2.5", "%E", 2.5f }, { "-1.25", "%F", -1.25f },
		{ "4e1", "%G", 40.0f },
	};
	float x;
	unsigned u = 0;
	int r;
	size_t i;

	pc_begin("XSH.fscanf.029");
	pc_test();
	for (i = 0; i < sizeof t / sizeof t[0]; i++) {
		x = -1;
		r = sscanf(t[i].in, t[i].fmt, &x);
		pc_check(r == 1 && near_f(x, t[i].want), "\"%s\" on \"%s\" returned %d and %g, expected 1 and %g",
		    t[i].fmt, t[i].in, r, (double)x, (double)t[i].want);
	}
	r = sscanf("1f", "%X", &u);
	pc_check(r == 1 && u == 0x1f, "\"%%X\" on \"1f\" returned %d and %#x, expected 1 and 0x1f", r, u);
	pc_pass();
}
