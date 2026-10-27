/*
 * A failed realloc() leaves the old block allocated.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.realloc.003
 */
#include "pc.h"
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	unsigned char *p, *q;
	size_t i;

	pc_begin("XSH.realloc.003");
	p = malloc(256);
	pc_check(p != NULL, "malloc(256) failed");
	for (i = 0; i < 256; i++)
		p[i] = (unsigned char)(255 - i);
	pc_test();
	errno = 0;
	q = realloc(p, SIZE_MAX);
	pc_check(q == NULL, "realloc(p, SIZE_MAX) returned a pointer");
	pc_check(errno == ENOMEM, "errno is %s, expected ENOMEM", pc_errname(errno));
	for (i = 0; i < 256; i++)
		pc_check(p[i] == (unsigned char)(255 - i), "byte %lu of the old block changed", (unsigned long)i);
	q = realloc(p, 512);
	pc_check(q != NULL, "the old block could not be resized after the failure");
	for (i = 0; i < 256; i++)
		pc_check(q[i] == (unsigned char)(255 - i), "byte %lu lost in the later resize", (unsigned long)i);
	free(q);
	pc_pass();
}
