/*
 * free() releases space from every allocation function.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.free.003
 */
#include "pc.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	void *a, *b, *c, *d, *e;
	int i;

	pc_begin("XSH.free.003");
	pc_test();
	for (i = 0; i < 100; i++) {
		a = calloc(10, 10);
		b = realloc(NULL, 100);
		b = realloc(b, 300);
		c = aligned_alloc(_Alignof(max_align_t), 64);
		d = calloc(100, 1);
		e = reallocarray(NULL, 5, 20);
		pc_check(a && b && c && d && e, "an allocation failed");
		memset(a, 1, 100);
		memset(b, 2, 300);
		memset(c, 3, 64);
		memset(d, 4, 100);
		memset(e, 5, 100);
		free(a);
		free(b);
		free(c);
		free(d);
		free(e);
	}
	{
		char *q = malloc(100);

		pc_check(q != NULL, "malloc failed after freeing");
		memset(q, 9, 100);
		free(q);
	}
	pc_pass();
}
