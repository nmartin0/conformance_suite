/*
 * sscanf() reads from the string it is given.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.003
 * MUTANT    fscanf.tokenizes
 * MUTANT    fscanf.float_truncates
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
	char s[] = "25 54.32E-1 Hamster", copy[sizeof s], name[50] = "";
	int i = -1, r;
	float x = -1;

	pc_begin("XSH.fscanf.003");
	memcpy(copy, s, sizeof s);
	pc_test();
	r = sscanf(s, "%d%f%s", &i, &x, name);
	pc_check(r == 3, "sscanf() returned %d, expected 3", r);
	pc_check(i == 25, "%%d stored %d, expected 25", i);
	pc_check(near_f(x, 5.432f), "%%f stored %g, expected 5.432", (double)x);
	pc_check(strcmp(name, "Hamster") == 0, "%%s stored \"%s\", expected \"Hamster\"", name);
	pc_check(memcmp(s, copy, sizeof s) == 0, "the input string was changed");
	pc_pass();
}
