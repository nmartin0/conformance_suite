/*
 * posix_memalign() reports ENOMEM through its return value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_memalign.003
 */
#include "pc.h"
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

int main(void)
{
	int dummy;
	void *sentinel = &dummy, *p = sentinel;
	int rc;

	pc_begin("XSH.posix_memalign.003");
	pc_test();
	rc = posix_memalign(&p, 2 * sizeof(void *), SIZE_MAX);
	pc_check(rc == ENOMEM, "returned %d, expected ENOMEM", rc);
	pc_check(p == sentinel || p == NULL, "*memptr was set to %p", p);
	pc_pass();
}
