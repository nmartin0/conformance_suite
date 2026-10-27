/*
 * calloc() space is zeroed even when the heap held other data.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.calloc.001
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>

int main(void)
{
	static const size_t sizes[] = { 8, 24, 100, 1000, 5000, 70000 };
	size_t i, j;

	pc_begin("XSH.calloc.001");
	pc_test();
	for (i = 0; i < sizeof sizes / sizeof sizes[0]; i++) {
		size_t n = sizes[i];
		unsigned char *d = malloc(n * 4), *c;

		pc_check(d != NULL, "malloc failed");
		memset(d, 0xA5, n * 4);
		free(d);
		c = calloc(n, 4);
		pc_check(c != NULL, "calloc(%lu, 4) returned a null pointer", (unsigned long)n);
		for (j = 0; j < n * 4; j++)
			pc_check(c[j] == 0, "byte %lu of calloc(%lu, 4) is %#x, not 0",
			    (unsigned long)j, (unsigned long)n, c[j]);
		memset(c, 0x5A, n * 4);
		free(c);
	}
	pc_pass();
}
