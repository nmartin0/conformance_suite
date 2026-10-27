/*
 * aligned_alloc() rejects an alignment that is not a power of two.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.aligned_alloc.002
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>

int main(void)
{
	static const size_t al[] = { 3, 24, 100 };
	size_t i;

	pc_begin("XSH.aligned_alloc.002");
	pc_test();
	for (i = 0; i < 3; i++) {
		void *p;

		errno = 0;
		p = aligned_alloc(al[i], al[i] * 2);
		pc_check(p == NULL, "aligned_alloc(%lu, %lu) returned a pointer", (unsigned long)al[i], (unsigned long)al[i] * 2);
		pc_check(errno == EINVAL, "alignment %lu: errno is %s, expected EINVAL", (unsigned long)al[i], pc_errname(errno));
	}
	pc_pass();
}
