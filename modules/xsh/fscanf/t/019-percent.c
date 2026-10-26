/*
 * %% matches one '%' after white space and assigns nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.019
 * MUTANT    fscanf.percent_no_skip
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
	int x = -1, r;

	pc_begin("XSH.fscanf.019");
	pc_test();
	r = sscanf("%7", "%%%d", &x);
	pc_check(r == 1 && x == 7, "sscanf(\"%%7\", \"%%%%%%d\") returned %d and %d, expected 1 and 7", r, x);
	x = -1;
	r = sscanf(" \t%7", "%%%d", &x);
	pc_check(r == 1 && x == 7, "sscanf(\" \\t%%7\", \"%%%%%%d\") returned %d and %d, expected 1 and 7", r, x);
	r = sscanf("x7", "%%%d", &x);
	pc_check(r == 0, "sscanf(\"x7\", \"%%%%%%d\") returned %d, expected 0", r);
	pc_pass();
}
