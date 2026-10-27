/*
 * realloc() keeps the old contents when growing and shrinking.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.realloc.002
 */
#include "pc.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	unsigned char *p, *q, *blocker[16];
	size_t i;

	pc_begin("XSH.realloc.002");
	pc_test();
	p = malloc(100);
	pc_check(p != NULL, "malloc(100) failed");
	for (i = 0; i < 100; i++)
		p[i] = (unsigned char)(i * 3 + 1);
	/* Allocate neighbours so that growing in place is unlikely. */
	for (i = 0; i < 16; i++)
		blocker[i] = malloc(100);
	q = realloc(p, 5000);
	pc_check(q != NULL, "realloc(p, 5000) returned a null pointer");
	pc_check(((uintptr_t)q % _Alignof(max_align_t)) == 0, "grown block %p is not aligned", (void *)q);
	for (i = 0; i < 100; i++)
		pc_check(q[i] == (unsigned char)(i * 3 + 1), "byte %lu changed after growing", (unsigned long)i);
	memset(q + 100, 0x77, 4900);
	p = realloc(q, 40);
	pc_check(p != NULL, "realloc(q, 40) returned a null pointer");
	pc_check(((uintptr_t)p % _Alignof(max_align_t)) == 0, "shrunk block %p is not aligned", (void *)p);
	for (i = 0; i < 40; i++)
		pc_check(p[i] == (unsigned char)(i * 3 + 1), "byte %lu changed after shrinking", (unsigned long)i);
	q = realloc(p, 40);	/* same size */
	pc_check(q != NULL, "realloc to the same size returned a null pointer");
	for (i = 0; i < 40; i++)
		pc_check(q[i] == (unsigned char)(i * 3 + 1), "byte %lu changed on a same-size realloc", (unsigned long)i);
	free(q);
	for (i = 0; i < 16; i++)
		free(blocker[i]);
	pc_pass();
}
