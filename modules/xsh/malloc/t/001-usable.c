/*
 * malloc() returns usable space.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.malloc.001
 */
#include "pc.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	static const size_t sizes[] = { 1, 2, 7, 8, 15, 16, 17, 100, 1000, 4096, 65536, 1048576 };
	size_t i, j;

	pc_begin("XSH.malloc.001");
	pc_test();
	for (i = 0; i < sizeof sizes / sizeof sizes[0]; i++) {
		unsigned char *p = malloc(sizes[i]);

		pc_check(p != NULL, "malloc(%lu) returned a null pointer", (unsigned long)sizes[i]);
		for (j = 0; j < sizes[i]; j++)
			p[j] = (unsigned char)(j * 7 + i);
		for (j = 0; j < sizes[i]; j++)
			pc_check(p[j] == (unsigned char)(j * 7 + i),
			    "byte %lu of a %lu byte block did not keep its value", (unsigned long)j, (unsigned long)sizes[i]);
		free(p);
	}
	pc_pass();
}
