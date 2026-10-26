/*
 * The [, c and n conversions do not skip white space.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.011
 * MUTANT    fscanf.always_skip
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
	int n = -1, r;
	char c = 0, buf[8] = "";

	pc_begin("XSH.fscanf.011");
	pc_test();
	r = sscanf("ab  cd", "ab%n%c", &n, &c);
	pc_check(r == 1, "sscanf(\"ab  cd\", \"ab%%n%%c\") returned %d, expected 1", r);
	pc_check(n == 2, "%%n stored %d, expected 2", n);
	pc_check(c == ' ', "%%c stored %#x, expected a space", (unsigned)c);
	r = sscanf("  xy", "%[ xy]", buf);
	pc_check(r == 1 && strcmp(buf, "  xy") == 0, "sscanf(\"  xy\", \"%%[ xy]\") returned %d and \"%s\"", r, buf);
	pc_pass();
}
