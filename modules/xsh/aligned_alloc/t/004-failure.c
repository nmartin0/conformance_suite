/*
 * aligned_alloc() fails with ENOMEM for an impossible size.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.aligned_alloc.004
 */
#include "pc.h"
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

int main(void)
{
	void *p;

	pc_begin("XSH.aligned_alloc.004");
	pc_test();
	errno = 0;
	p = aligned_alloc(_Alignof(max_align_t), SIZE_MAX & ~(size_t)(_Alignof(max_align_t) - 1));
	pc_check(p == NULL, "aligned_alloc(16, huge) returned a pointer");
	pc_check(errno == ENOMEM, "errno is %s, expected ENOMEM", pc_errname(errno));
	pc_pass();
}
