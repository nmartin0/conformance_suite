/*
 * reallocarray() with a zero count or element size.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.realloc.007
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>

int main(void)
{
	void *p, *q;
	int k;

	pc_begin("XSH.realloc.007");
	pc_test();
	for (k = 0; k < 2; k++) {
		p = malloc(32);
		pc_check(p != NULL, "malloc(32) failed");
		errno = 0;
		q = k ? reallocarray(p, 5, 0) : reallocarray(p, 0, 5);
		if (q == NULL)
			pc_check(errno == EINVAL, "null result with errno %s, expected EINVAL", pc_errname(errno));
		else
			free(q);
	}
	pc_pass();
}
