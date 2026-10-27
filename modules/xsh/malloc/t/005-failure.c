/*
 * malloc() reports failure with a null pointer and ENOMEM.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.malloc.005
 */
#include "pc.h"
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

int main(void)
{
	void *p;

	pc_begin("XSH.malloc.005");
	pc_test();
	errno = 0;
	p = malloc(SIZE_MAX);
	pc_check(p == NULL, "malloc(SIZE_MAX) returned a pointer");
	pc_check(errno == ENOMEM, "errno is %s, expected ENOMEM", pc_errname(errno));
	pc_pass();
}
