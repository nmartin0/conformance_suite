/*
 * %p reads back what snprintf() wrote with %p for a null pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.036
 * MUTANT    fscanf.p_null_nonzero
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
	char b[64];
	void *q;
	int r;

	pc_begin("XSH.fscanf.036");
	(void)snprintf(b, sizeof b, "%p", (void *)0);
	pc_test();
	q = &q;
	r = sscanf(b, "%p", &q);
	pc_check(r == 1, "\"%%p\" on \"%s\" (a null pointer) returned %d, expected 1", b, r);
	pc_check(q == NULL, "\"%%p\" on \"%s\" stored %p, expected a null pointer", b, q);
	pc_pass();
}
