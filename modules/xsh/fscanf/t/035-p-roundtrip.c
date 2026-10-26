/*
 * %p reads back what snprintf() wrote with %p.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.035
 * MUTANT    fscanf.p_decimal
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
	static int obj;
	char b1[64], b2[64];
	void *h, *q;
	int r;

	pc_begin("XSH.fscanf.035");
	h = malloc(16);
	pc_check(h != NULL, "malloc() failed");
	(void)snprintf(b1, sizeof b1, "%p", (void *)&obj);
	(void)snprintf(b2, sizeof b2, "%p", h);
	pc_test();
	q = NULL;
	r = sscanf(b1, "%p", &q);
	pc_check(r == 1 && q == (void *)&obj, "\"%%p\" on \"%s\" returned %d and %p", b1, r, q);
	q = NULL;
	r = sscanf(b2, "%p", &q);
	pc_check(r == 1 && q == h, "\"%%p\" on \"%s\" returned %d and %p", b2, r, q);
	pc_pass();
}
