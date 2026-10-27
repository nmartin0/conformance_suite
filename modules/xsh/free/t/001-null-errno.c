/*
 * free() leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.free.001
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>

int main(void)
{
	void *p;
	int i;

	pc_begin("XSH.free.001");
	pc_test();
	errno = EDOM;
	free(NULL);
	pc_check(errno == EDOM, "free(NULL) changed errno to %s", pc_errname(errno));
	for (i = 0; i < 20; i++) {
		p = malloc((size_t)1 << i);
		pc_check(p != NULL, "malloc failed");
		errno = ERANGE;
		free(p);
		pc_check(errno == ERANGE, "free() of a malloc() pointer changed errno to %s", pc_errname(errno));
	}
	pc_pass();
}
