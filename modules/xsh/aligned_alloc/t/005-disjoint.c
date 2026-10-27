/*
 * Live aligned_alloc() blocks do not overlap.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.aligned_alloc.005
 */
#include "pc.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define N 32

int main(void)
{
	unsigned char *p[N];
	size_t n[N], i, j;
	size_t a = _Alignof(max_align_t);

	pc_begin("XSH.aligned_alloc.005");
	pc_test();
	for (i = 0; i < N; i++) {
		size_t al = a;

		n[i] = al * (1 + i % 5);
		p[i] = aligned_alloc(al, n[i]);
		pc_check(p[i] != NULL, "aligned_alloc(%lu, %lu) returned a null pointer", (unsigned long)al, (unsigned long)n[i]);
		pc_check(((uintptr_t)p[i] % al) == 0, "result %p is not aligned to %lu", (void *)p[i], (unsigned long)al);
		memset(p[i], (int)(i + 1), n[i]);
	}
	for (i = 0; i < N; i++)
		for (j = 0; j < n[i]; j++)
			pc_check(p[i][j] == (unsigned char)(i + 1), "block %lu was overwritten", (unsigned long)i);
	for (i = 0; i < N; i++)
		for (j = i + 1; j < N; j++)
			pc_check((uintptr_t)p[i] + n[i] <= (uintptr_t)p[j] || (uintptr_t)p[j] + n[j] <= (uintptr_t)p[i],
			    "blocks %lu and %lu overlap", (unsigned long)i, (unsigned long)j);
	for (i = 0; i < N; i++)
		free(p[i]);
	pc_pass();
}
