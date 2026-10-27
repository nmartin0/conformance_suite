/*
 * aligned_alloc() returns suitably aligned space.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.aligned_alloc.001
 */
#include "pc.h"
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	static const size_t fund[] = {
		_Alignof(char), _Alignof(short), _Alignof(int), _Alignof(long), _Alignof(long long),
		_Alignof(double), _Alignof(long double), _Alignof(void *), _Alignof(max_align_t),
	};
	static const size_t ext[] = { 64, 4096 };
	size_t i, j;

	pc_begin("XSH.aligned_alloc.001");
	pc_test();
	for (i = 0; i < sizeof fund / sizeof fund[0]; i++) {
		size_t a = fund[i], n = a * 8;
		unsigned char *p = aligned_alloc(a, n);

		pc_check(p != NULL, "aligned_alloc(%lu, %lu) returned a null pointer", (unsigned long)a, (unsigned long)n);
		pc_check(((uintptr_t)p % a) == 0, "result %p is not a multiple of %lu", (void *)p, (unsigned long)a);
		for (j = 0; j < n; j++)
			p[j] = (unsigned char)(j + 5);
		for (j = 0; j < n; j++)
			pc_check(p[j] == (unsigned char)(j + 5), "byte %lu did not keep its value", (unsigned long)j);
		free(p);
	}
	for (i = 0; i < 2; i++) {
		size_t a = ext[i], n = a * 2;
		unsigned char *p;

		errno = 0;
		p = aligned_alloc(a, n);
		if (p == NULL) {
			pc_check(errno == EINVAL || errno == ENOMEM, "alignment %lu: null result with errno %s",
			    (unsigned long)a, pc_errname(errno));
			continue;
		}
		pc_check(((uintptr_t)p % a) == 0, "result %p is not a multiple of %lu", (void *)p, (unsigned long)a);
		memset(p, 0x42, n);
		free(p);
	}
	pc_pass();
}
