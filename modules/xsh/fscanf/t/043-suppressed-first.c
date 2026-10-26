/*
 * A completed suppressed conversion counts as the first conversion.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.043
 * MUTANT    fscanf.eof_unless_assigned
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

	pc_begin("XSH.fscanf.043");
	pc_test();
	r = sscanf("1", "%*d%d", &x);
	pc_check(r == 0, "sscanf(\"1\", \"%%*d%%d\") returned %d, expected 0 (the %%*d conversion completed)", r);
	pc_pass();
}
