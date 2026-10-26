/*
 * In the POSIX locale the radix character is the period.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.006
 * MUTANT    fscanf.radix_comma
 * VARIANT   fscanf.float_ulp
 */
#include "pc.h"
#include <errno.h>
#include <float.h>
#include <locale.h>
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
	float x = -1;
	int n = -1, r;

	pc_begin("XSH.fscanf.006");
	pc_check(setlocale(LC_ALL, "POSIX") != NULL, "setlocale(LC_ALL, \"POSIX\") failed");
	pc_test();
	r = sscanf("1.5", "%f", &x);
	pc_check(r == 1 && near_f(x, 1.5f), "sscanf(\"1.5\", \"%%f\") returned %d and %g", r, (double)x);
	x = -1;
	r = sscanf("2,5", "%f%n", &x, &n);
	pc_check(r == 1, "sscanf(\"2,5\", \"%%f%%n\") returned %d, expected 1", r);
	pc_check(near_f(x, 2.0f), "stored %g, expected 2", (double)x);
	pc_check(n == 1, "%%n counted %d bytes, expected 1 (the comma is not a radix)", n);
	pc_pass();
}
