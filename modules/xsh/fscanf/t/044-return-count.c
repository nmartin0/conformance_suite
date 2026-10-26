/*
 * The return value counts the items matched and assigned.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.044
 * MUTANT    fscanf.count_failed
 * MUTANT    fscanf.matching_returns_eof
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
	int a = -1, b = -1, c = -1, r;

	pc_begin("XSH.fscanf.044");
	pc_test();
	r = sscanf("1 2 x", "%d %d %d", &a, &b, &c);
	pc_check(r == 2 && a == 1 && b == 2, "sscanf(\"1 2 x\", \"%%d %%d %%d\") returned %d (%d %d), expected 2 (1 2)",
	    r, a, b);
	r = sscanf("x", "%d", &a);
	pc_check(r == 0, "sscanf(\"x\", \"%%d\") returned %d, expected 0", r);
	pc_pass();
}
