/*
 * memchr: errno kept.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memchr.006
 */
#include "pc.h"
#include <string.h>
#include <errno.h>
static void *(*volatile f_memchr)(const void *, int, size_t) = memchr;

int main(void)
{
	int err;
	static const char s[3] = "ab";
	pc_begin("XSH.memchr.006");
	pc_test();
	errno = EDOM;
	(void)f_memchr(s, 'b', 3);
	err = errno;
	pc_check(err == EDOM, "memchr() on a present byte changed errno to %s", pc_errname(err));
	errno = EDOM;
	(void)f_memchr(s, 'z', 3);
	err = errno;
	pc_check(err == EDOM, "memchr() on an absent byte changed errno to %s", pc_errname(err));
	pc_pass();
}
