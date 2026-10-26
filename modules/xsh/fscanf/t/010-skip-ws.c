/*
 * Conversions other than [, c and n skip leading white space.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.010
 * MUTANT    fscanf.no_skip
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
	int i = -1, r;
	unsigned u = 0;
	float x = -1;
	char buf[8] = "";

	pc_begin("XSH.fscanf.010");
	pc_test();
	r = sscanf("  7", "%d", &i);
	pc_check(r == 1 && i == 7, "sscanf(\"  7\", \"%%d\") returned %d and %d", r, i);
	r = sscanf("\t\n hi", "%s", buf);
	pc_check(r == 1 && strcmp(buf, "hi") == 0, "sscanf(\"\\t\\n hi\", \"%%s\") returned %d and \"%s\"", r, buf);
	r = sscanf("  1.5", "%f", &x);
	pc_check(r == 1 && near_f(x, 1.5f), "sscanf(\"  1.5\", \"%%f\") returned %d and %g", r, (double)x);
	r = sscanf(" \n1f", "%x", &u);
	pc_check(r == 1 && u == 0x1f, "sscanf(\" \\n1f\", \"%%x\") returned %d and %#x", r, u);
	pc_pass();
}
