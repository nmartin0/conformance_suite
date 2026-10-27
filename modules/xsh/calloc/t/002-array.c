/*
 * calloc() returns aligned, disjoint space of nelem*elsize bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.calloc.002
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

	pc_begin("XSH.calloc.002");
	pc_test();
	for (i = 0; i < N; i++) {
		size_t ne = 1 + i % 7, es = 1 + (i * 5) % 13;

		n[i] = ne * es;
		p[i] = calloc(ne, es);
		pc_check(p[i] != NULL, "calloc(%lu, %lu) returned a null pointer", (unsigned long)ne, (unsigned long)es);
		pc_check(((uintptr_t)p[i] % _Alignof(max_align_t)) == 0, "calloc(%lu, %lu) result %p is not aligned",
		    (unsigned long)ne, (unsigned long)es, (void *)p[i]);
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
