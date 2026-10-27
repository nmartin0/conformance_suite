/*
 * memmove: errno kept.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memmove.007
 */
#include "pc.h"
#include <string.h>
#include <errno.h>
static void *(*volatile f_memmove)(void *, const void *, size_t) = memmove;

int main(void)
{
	int err;
	char b[8] = "abcdefg", c[8];
	pc_begin("XSH.memmove.007");
	pc_test();
	errno = EDOM;
	(void)f_memmove(c, b, 8);
	err = errno;
	pc_check(err == EDOM, "memmove() of disjoint objects changed errno to %s", pc_errname(err));
	errno = EDOM;
	(void)f_memmove(b + 1, b, 5);
	err = errno;
	pc_check(err == EDOM, "memmove() of overlapping objects changed errno to %s", pc_errname(err));
	pc_pass();
}
