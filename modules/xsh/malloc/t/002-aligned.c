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

/*
 * The alignment that every block of n bytes must have: that of the most
 * demanding basic type whose size fits in n (the text asks for access to an
 * object of fundamental alignment "in the space allocated", which a type
 * larger than the block cannot be).
 */
static size_t need_align(size_t n)
{
	size_t a = 1;

#define FIT(T) do { if (sizeof(T) <= n && _Alignof(T) > a) a = _Alignof(T); } while (0)
	FIT(short);
	FIT(int);
	FIT(long);
	FIT(long long);
	FIT(float);
	FIT(double);
	FIT(long double);
	FIT(void *);
	FIT(max_align_t);
#undef FIT
	return a;
}

int main(void)
{
	void *keep[40];
	size_t i;

	pc_begin("XSH.malloc.002");
	pc_test();
	for (i = 0; i < 40; i++) {
		size_t n = i < 20 ? i + 1 : (i - 19) * 24;
		size_t a = need_align(n);
		void *p = malloc(n);

		pc_check(p != NULL, "malloc(%lu) returned a null pointer", (unsigned long)n);
		pc_check(((uintptr_t)p % a) == 0,
		    "malloc(%lu) returned %p, not a multiple of %lu, the alignment of the widest basic type that fits", (unsigned long)n, p, (unsigned long)a);
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
