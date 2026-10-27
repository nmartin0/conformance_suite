/*
 * memcpy: errno kept.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memcpy.006
 */
#include "pc.h"
#include <string.h>
#include <errno.h>
static void *(*volatile f_memcpy)(void *restrict, const void *restrict, size_t) = memcpy;

int main(void)
{
	int err;
	char src[4] = "abc", dst[4];
	pc_begin("XSH.memcpy.006");
	pc_test();
	errno = EDOM;
	(void)f_memcpy(dst, src, 4);
	err = errno;
	pc_check(err == EDOM, "memcpy() changed errno to %s", pc_errname(err));
	errno = EDOM;
	(void)f_memcpy(dst, src, 0);
	err = errno;
	pc_check(err == EDOM, "memcpy() with n 0 changed errno to %s", pc_errname(err));
	pc_pass();
}
