/*
 * reallocarray() equals realloc() of nelem * elsize.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.realloc.005
 */
#include "pc.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	unsigned char *p, *q;
	size_t i;

	pc_begin("XSH.realloc.005");
	pc_test();
	p = reallocarray(NULL, 10, 12);
	pc_check(p != NULL, "reallocarray(NULL, 10, 12) returned a null pointer");
	pc_check(((uintptr_t)p % _Alignof(max_align_t)) == 0, "result %p is not aligned", (void *)p);
	for (i = 0; i < 120; i++)
		p[i] = (unsigned char)(i + 9);
	q = reallocarray(p, 100, 50);
	pc_check(q != NULL, "reallocarray(p, 100, 50) returned a null pointer");
	for (i = 0; i < 120; i++)
		pc_check(q[i] == (unsigned char)(i + 9), "byte %lu changed on growing", (unsigned long)i);
	memset(q + 120, 1, 4880);
	p = reallocarray(q, 7, 3);
	pc_check(p != NULL, "reallocarray(q, 7, 3) returned a null pointer");
	for (i = 0; i < 21; i++)
		pc_check(p[i] == (unsigned char)(i + 9), "byte %lu changed on shrinking", (unsigned long)i);
	free(p);
	pc_pass();
}
