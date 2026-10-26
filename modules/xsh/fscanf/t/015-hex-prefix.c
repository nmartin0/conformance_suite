/*
 * The input item "0x" of "0xg" is not a number.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.015
 * MUTANT    fscanf.hex_backtrack
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
	unsigned u = 7;
	int i = 7, n = -1, r;

	pc_begin("XSH.fscanf.015");
	pc_test();
	r = sscanf("0xg", "%x%n", &u, &n);
	pc_check(r == 0, "sscanf(\"0xg\", \"%%x%%n\") returned %d, expected 0", r);
	pc_check(n == -1, "%%n was reached after %%x (it stored %d)", n);
	r = sscanf("0xg", "%i%n", &i, &n);
	pc_check(r == 0, "sscanf(\"0xg\", \"%%i%%n\") returned %d, expected 0", r);
	pc_check(n == -1, "%%n was reached after %%i (it stored %d)", n);
	pc_pass();
}
