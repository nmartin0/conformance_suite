/*
 * posix_memalign() aligned allocation.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_memalign.001
 */
#include "pc.h"
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	size_t a, j;

	pc_begin("XSH.posix_memalign.001");
	pc_test();
	for (a = sizeof(void *); a <= 4096; a *= 2) {
		unsigned char *p = NULL;
		size_t n = 3 * a + 1;
		int rc = posix_memalign((void **)&p, a, n);

		if (a > 64 && rc == ENOMEM)
			continue;
		pc_check(rc == 0, "posix_memalign(alignment %lu) returned %d", (unsigned long)a, rc);
		pc_check(p != NULL, "alignment %lu: null pointer stored for a non-zero size", (unsigned long)a);
		pc_check(((uintptr_t)p % a) == 0, "alignment %lu: pointer %p is not a multiple", (unsigned long)a, (void *)p);
		for (j = 0; j < n; j++)
			p[j] = (unsigned char)(j * 11 + 1);
		for (j = 0; j < n; j++)
			pc_check(p[j] == (unsigned char)(j * 11 + 1), "alignment %lu: byte %lu changed", (unsigned long)a, (unsigned long)j);
		free(p);
	}
	pc_pass();
}
