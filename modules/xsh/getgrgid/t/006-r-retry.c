/*
 * Doubling the buffer after ERANGE ends with the entry.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getgrgid.006
 */
#include "grdb.h"

#include <stdlib.h>

int main(void)
{
	struct xc c, d, f;
	xent_t e, other, *res, *p;
	char *buf;
	size_t n;
	int r;

	pc_begin("XSH.getgrgid.006");
	xc_find(&c);
	p = getgrgid(c.id);
	pc_check(p != NULL, "no entry found for ID %ld", (long)c.id);
	xc_set(&d, p);
	memset(&e, 0, sizeof e);
	pc_test();
	for (n = 1;; n *= 2) {
		if (n > (size_t)1 << 20)
			pc_unresolved("still ERANGE with 1 MiB of buffer");
		buf = malloc(n);
		if (buf == NULL)
			pc_unresolved("malloc failed");
		res = &other;
		r = getgrgid_r(c.id, &e, buf, n, &res);
		if (r != ERANGE)
			break;
		pc_check(res == NULL, "*result is not null after ERANGE (size %lu)", (unsigned long)n);
		free(buf);
	}
	pc_check(r == 0, "returned %d (%s) with %lu bytes", r, pc_errname(r), (unsigned long)n);
	pc_check(res == &e, "*result is not the structure passed");
	xc_set(&f, &e);
	pc_check(xc_same(&f, &d), "the entry differs from the one the plain lookup gives");
	pc_check(xc_inbuf(&e, buf, n), "the entry refers to storage outside the buffer");
	free(buf);
	pc_pass();
}
