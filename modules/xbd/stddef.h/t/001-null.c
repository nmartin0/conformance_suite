/*
 * NULL is a null pointer constant of type void *; all-zero pointer objects are null.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.stddef.h.001
 */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "pc.h"

_Static_assert(_Generic(NULL, void *: 1, default: 0), "NULL does not have type void *");

struct s { int a; };

int main(void)
{
	pc_begin("XBD.stddef.h.001");
	pc_test();
	char *c;
	int *i;
	void (*f)(void);
	struct s *t;
	void *v;
	void **heap;

	pc_check(NULL == 0, "NULL does not compare equal to 0");
	pc_check(!NULL, "NULL is not false");
	memset(&c, 0, sizeof c);
	memset(&i, 0, sizeof i);
	memset(&f, 0, sizeof f);
	memset(&t, 0, sizeof t);
	memset(&v, 0, sizeof v);
	pc_check(c == NULL && i == NULL && f == NULL && t == NULL && v == NULL, "a pointer whose bytes are all zero is not null");
	heap = calloc(4, sizeof *heap);
	if (heap == NULL)
		pc_unresolved("calloc failed");
	pc_check(heap[0] == NULL && heap[3] == NULL, "a pointer in calloc()ed memory is not null");
	free(heap);
	pc_pass();
}
