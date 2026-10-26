/*
 * %n$ conversions store into the nth argument.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.005
 * MUTANT    fscanf.numbered_ignored
 * MUTANT    fscanf.percent_counted
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
	int a = -1, b = -1, r;

	pc_begin("XSH.fscanf.005");
	pc_test();
	r = sscanf("10 20", "%2$d %1$d", &a, &b);
	pc_check(r == 2, "sscanf(\"10 20\", \"%%2$d %%1$d\") returned %d, expected 2", r);
	pc_check(a == 20 && b == 10, "stored %d and %d, expected 20 in the first and 10 in the second", a, b);
	a = -1;
	r = sscanf("% 7 8", "%%%*d%1$d", &a);
	pc_check(r == 1, "sscanf(\"%% 7 8\", \"%%%%%%*d%%1$d\") returned %d, expected 1", r);
	pc_check(a == 8, "stored %d, expected 8", a);
	pc_pass();
}
