/*
 * reallocarray() fails on overflow and keeps the old block.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.realloc.006
 */
#include "pc.h"
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

int main(void)
{
	static const size_t big[][2] = {
		{ SIZE_MAX / 2 + 1, 2 }, { 2, SIZE_MAX / 2 + 1 }, { SIZE_MAX, 2 }, { SIZE_MAX, SIZE_MAX },
		{ SIZE_MAX / 3 + 1, 3 },
	};
	unsigned char *p;
	size_t i, k;

	pc_begin("XSH.realloc.006");
	p = malloc(64);
	pc_check(p != NULL, "malloc(64) failed");
	for (i = 0; i < 64; i++)
		p[i] = (unsigned char)(i ^ 0x5c);
	pc_test();
	for (k = 0; k < sizeof big / sizeof big[0]; k++) {
		void *q;

		errno = 0;
		q = reallocarray(p, big[k][0], big[k][1]);
		pc_check(q == NULL, "reallocarray(p, %zu, %zu) overflows but returned a pointer", big[k][0], big[k][1]);
		pc_check(errno == ENOMEM, "errno is %s, expected ENOMEM", pc_errname(errno));
		for (i = 0; i < 64; i++)
			pc_check(p[i] == (unsigned char)(i ^ 0x5c), "old block changed after a failed reallocarray()");
	}
	free(p);
	pc_pass();
}
