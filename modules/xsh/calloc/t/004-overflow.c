/*
 * calloc() detects overflow of nelem * elsize.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.calloc.004
 */
#include "pc.h"
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

int main(void)
{
	static const size_t big[][2] = {
		{ SIZE_MAX / 2 + 1, 2 }, { 2, SIZE_MAX / 2 + 1 }, { SIZE_MAX, 2 },
		{ SIZE_MAX, SIZE_MAX }, { SIZE_MAX / 3 + 1, 3 }, { (size_t)1 << (sizeof(size_t) * 4), (size_t)1 << (sizeof(size_t) * 4) },
	};
	size_t i;

	pc_begin("XSH.calloc.004");
	pc_test();
	for (i = 0; i < sizeof big / sizeof big[0]; i++) {
		void *p;

		errno = 0;
		p = calloc(big[i][0], big[i][1]);
		pc_check(p == NULL, "calloc(%zu, %zu) overflows but returned a pointer", big[i][0], big[i][1]);
		pc_check(errno == ENOMEM, "calloc(%zu, %zu): errno is %s, expected ENOMEM",
		    big[i][0], big[i][1], pc_errname(errno));
	}
	pc_pass();
}
