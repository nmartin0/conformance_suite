/*
 * realloc(p, 0): null with EINVAL, or a pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.realloc.004
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>

int main(void)
{
	void *p, *q;

	pc_begin("XSH.realloc.004");
	p = malloc(32);
	pc_check(p != NULL, "malloc(32) failed");
	pc_test();
	errno = 0;
	q = realloc(p, 0);
	if (q == NULL)
		pc_check(errno == EINVAL, "realloc(p, 0) returned null with errno %s, expected EINVAL", pc_errname(errno));
	else
		free(q);
	pc_pass();
}
