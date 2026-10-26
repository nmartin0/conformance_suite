/*
 * %d reads a decimal integer as strtol() with base 10.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.024
 * MUTANT    fscanf.d_as_i
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
	static const struct { const char *in; int want; } t[] = {
		{ "-12", -12 }, { "+7", 7 }, { "007", 7 },
	};
	char buf[8] = "";
	int x, r;
	size_t i;

	pc_begin("XSH.fscanf.024");
	pc_test();
	for (i = 0; i < sizeof t / sizeof t[0]; i++) {
		x = -1;
		r = sscanf(t[i].in, "%d", &x);
		pc_check(r == 1 && x == t[i].want, "\"%%d\" on \"%s\" returned %d and %d, expected 1 and %d",
		    t[i].in, r, x, t[i].want);
	}
	x = -1;
	r = sscanf("0x10", "%d%s", &x, buf);
	pc_check(r == 2 && x == 0 && strcmp(buf, "x10") == 0,
	    "\"%%d%%s\" on \"0x10\" returned %d, %d and \"%s\", expected 2, 0 and \"x10\"", r, x, buf);
	pc_pass();
}
