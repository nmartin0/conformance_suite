/*
 * Arguments the format does not use are evaluated and ignored.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.004
 * MUTANT    fscanf.writes_extra
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
	int a = -1, b = -7, c = -9, k = 0, r;

	pc_begin("XSH.fscanf.004");
	pc_test();
	r = sscanf("5 6", "%d", &a, &b, (k++, &c));
	pc_check(r == 1, "sscanf() returned %d, expected 1", r);
	pc_check(a == 5, "stored %d, expected 5", a);
	pc_check(b == -7 && c == -9, "an excess argument was written: %d %d", b, c);
	pc_check(k == 1, "the excess argument was evaluated %d times", k);
	pc_pass();
}
