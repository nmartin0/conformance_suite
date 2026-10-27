/*
 * malloc() results are suitably aligned.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.malloc.002
 */
#include "pc.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	void *keep[40];
	size_t i;

	pc_begin("XSH.malloc.002");
	pc_test();
	for (i = 0; i < 40; i++) {
		size_t n = i < 20 ? i + 1 : (i - 19) * 24;
		void *p = malloc(n);

		pc_check(p != NULL, "malloc(%lu) returned a null pointer", (unsigned long)n);
		pc_check(((uintptr_t)p % _Alignof(max_align_t)) == 0,
		    "malloc(%lu) returned %p, not a multiple of the fundamental alignment", (unsigned long)n, p);
		keep[i] = p;
	}
	for (i = 0; i < 40; i++)
		free(keep[i]);

	/* A block of the largest basic types, stored and read back. */
	{
		struct blk { long double ld; double d; long long ll; void *vp; } *b = malloc(sizeof *b);

		pc_check(b != NULL, "malloc(sizeof struct) returned a null pointer");
		b->ld = 1.25L;
		b->d = -2.5;
		b->ll = -123456789012345LL;
		b->vp = b;
		pc_check(b->ld == 1.25L && b->d == -2.5 && b->ll == -123456789012345LL && b->vp == (void *)b,
		    "values stored through the pointer were not read back");
		free(b);
	}
	pc_pass();
}
