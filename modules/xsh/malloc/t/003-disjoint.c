/*
 * Live malloc() allocations do not overlap.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.malloc.003
 */
#include "pc.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define N 64

int main(void)
{
	unsigned char *p[N];
	size_t n[N], i, j;

	pc_begin("XSH.malloc.003");
	pc_test();
	for (i = 0; i < N; i++) {
		n[i] = 1 + (i * 37) % 200;
		p[i] = malloc(n[i]);
		pc_check(p[i] != NULL, "malloc(%lu) returned a null pointer", (unsigned long)n[i]);
		memset(p[i], (int)(i + 1), n[i]);
	}
	for (i = 0; i < N; i++)
		for (j = 0; j < n[i]; j++)
			pc_check(p[i][j] == (unsigned char)(i + 1),
			    "block %lu was overwritten at byte %lu", (unsigned long)i, (unsigned long)j);
	for (i = 0; i < N; i++)
		for (j = i + 1; j < N; j++)
			pc_check((uintptr_t)p[i] + n[i] <= (uintptr_t)p[j] ||
			    (uintptr_t)p[j] + n[j] <= (uintptr_t)p[i],
			    "blocks %lu and %lu overlap", (unsigned long)i, (unsigned long)j);
	for (i = 0; i < N; i++)
		free(p[i]);
	pc_pass();
}
