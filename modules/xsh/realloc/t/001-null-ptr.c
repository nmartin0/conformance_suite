/*
 * realloc(NULL, n) is malloc(n).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.realloc.001
 */
#include "pc.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	static const size_t sizes[] = { 1, 9, 64, 1000, 100000 };
	size_t i, j;

	pc_begin("XSH.realloc.001");
	pc_test();
	for (i = 0; i < sizeof sizes / sizeof sizes[0]; i++) {
		unsigned char *p = realloc(NULL, sizes[i]);

		pc_check(p != NULL, "realloc(NULL, %lu) returned a null pointer", (unsigned long)sizes[i]);
		pc_check(((uintptr_t)p % _Alignof(max_align_t)) == 0, "result %p is not aligned", (void *)p);
		for (j = 0; j < sizes[i]; j++)
			p[j] = (unsigned char)(j + 3);
		for (j = 0; j < sizes[i]; j++)
			pc_check(p[j] == (unsigned char)(j + 3), "byte %lu did not keep its value", (unsigned long)j);
		free(p);
	}
	pc_pass();
}
