/*
 * The input item "100e" of "100ergs" is not a number.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.014
 * MUTANT    fscanf.float_prefix_backtrack
 * VARIANT   setvbuf.tiny_default
 */
#include "pc.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* A stream reading path (made in setup). */
static FILE *reader(const char *path)
{
	FILE *f = fopen(path, "r");

	NEED(f != NULL, "fopen(%s) failed: %s", path, pc_errname(errno));
	return f;
}

int main(void)
{
	FILE *f;
	float x = -1;
	int n = -1, r, c;

	pc_begin("XSH.fscanf.014");
	pc_make_file("f", "100ergs");
	pc_test();
	r = sscanf("100ergs", "%f%n", &x, &n);
	pc_check(r == 0, "sscanf(\"100ergs\", \"%%f%%n\") returned %d (stored %g), expected 0", r, (double)x);
	pc_check(n == -1, "%%n was reached (it stored %d) after a failed conversion", n);
	f = reader("f");
	r = fscanf(f, "%f", &x);
	pc_check(r == 0, "fscanf(\"%%f\") on \"100ergs\" returned %d, expected 0", r);
	c = fgetc(f);
	pc_check(c == 'r', "the next fgetc() returned %#x, expected 'r' (the byte after the item \"100e\")",
	    (unsigned)c);
	pc_pass();
}
