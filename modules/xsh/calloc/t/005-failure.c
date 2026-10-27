/*
 * calloc() fails with ENOMEM when no memory can satisfy the request.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.calloc.005
 */
#include "pc.h"
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

int main(void)
{
	void *p;

	pc_begin("XSH.calloc.005");
	pc_test();
	errno = 0;
	p = calloc(1, SIZE_MAX);
	pc_check(p == NULL, "calloc(1, SIZE_MAX) returned a pointer");
	pc_check(errno == ENOMEM, "errno is %s, expected ENOMEM", pc_errname(errno));
	errno = 0;
	p = calloc(SIZE_MAX, 1);
	pc_check(p == NULL, "calloc(SIZE_MAX, 1) returned a pointer");
	pc_check(errno == ENOMEM, "errno is %s, expected ENOMEM", pc_errname(errno));
	pc_pass();
}
