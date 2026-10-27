/*
 * posix_memalign() rejects bad alignments with EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_memalign.002
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>

int main(void)
{
	size_t al[4];
	size_t i;

	pc_begin("XSH.posix_memalign.002");
	al[0] = 1;
	al[1] = sizeof(void *) + 1;
	al[2] = 3 * sizeof(void *);
	al[3] = 0;
	pc_test();
	for (i = 0; i < 4; i++) {
		void *sentinel = (void *)&al, *p = sentinel;
		int rc;

		errno = 0;
		rc = posix_memalign(&p, al[i], 64);
		pc_check(rc == EINVAL, "alignment %lu: returned %d, expected EINVAL", (unsigned long)al[i], rc);
		pc_check(p == sentinel || p == NULL, "alignment %lu: *memptr was set to %p", (unsigned long)al[i], p);
	}
	pc_pass();
}
