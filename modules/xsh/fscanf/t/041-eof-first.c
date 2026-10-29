/*
 * End of input before any conversion matched returns EOF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.041
 * MUTANT    fscanf.eof_returns_zero
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
	char c = 0, buf[8] = "";

	pc_begin("XSH.fscanf.041");
	pc_test();
	r = sscanf("", "%d", &x);
	pc_check(r == EOF, "sscanf(\"\", \"%%d\") returned %d, expected EOF", r);
	r = sscanf("   ", "%d", &x);
	pc_check(r == EOF, "sscanf(\"   \", \"%%d\") returned %d, expected EOF", r);
	r = sscanf("", "%s", buf);
	pc_check(r == EOF, "sscanf(\"\", \"%%s\") returned %d, expected EOF", r);
	r = sscanf("", "%c", &c);
	pc_check(r == EOF, "sscanf(\"\", \"%%c\") returned %d, expected EOF", r);
	r = sscanf("", "%[a]", buf);
	pc_check(r == EOF, "sscanf(\"\", \"%%[a]\") returned %d, expected EOF", r);
	r = sscanf("  ", " %d", &x);
	pc_check(r == EOF, "sscanf(\"  \", \" %%d\") returned %d, expected EOF", r);
	/* Control: a function that answers EOF for everything is not conforming. */
	x = -1;
	r = sscanf("5", "%d", &x);
	pc_check(r == 1 && x == 5, "sscanf(\"5\", \"%%d\") returned %d with %d, expected 1 with 5", r, x);
	pc_pass();
}
