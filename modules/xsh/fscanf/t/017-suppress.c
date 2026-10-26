/*
 * '*' converts without assigning or counting.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.017
 * MUTANT    fscanf.suppressed_counted
 * MUTANT    fscanf.suppress_takes_arg
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
	int x = -1, y = -1, r;
	char buf[8] = "", spare[8] = "";

	pc_begin("XSH.fscanf.017");
	pc_test();
	/* the second pointer is an excess argument; it must stay untouched */
	r = sscanf("12 34", "%*d %d", &x, &y);
	pc_check(r == 1, "sscanf(\"12 34\", \"%%*d %%d\") returned %d, expected 1", r);
	pc_check(x == 34 && y == -1, "stored %d (and %d in the spare), expected 34 in the first", x, y);
	r = sscanf("ab cd ef", "%*s %*c%s", buf, spare);
	pc_check(r == 1, "sscanf(\"ab cd ef\", \"%%*s %%*c%%s\") returned %d, expected 1", r);
	pc_check(strcmp(buf, "d") == 0 && spare[0] == '\0', "stored \"%s\" (and \"%s\" in the spare), expected \"d\"",
	    buf, spare);
	pc_pass();
}
