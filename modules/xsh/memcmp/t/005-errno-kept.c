/*
 * memcmp: errno kept.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memcmp.005
 */
#include "pc.h"
#include <string.h>
#include <errno.h>
static int (*volatile f_memcmp)(const void *, const void *, size_t) = memcmp;

int main(void)
{
	int err;
	static const char s[3] = "ab", t[3] = "ac";
	pc_begin("XSH.memcmp.005");
	pc_test();
	errno = EDOM;
	(void)f_memcmp(s, s, 3);
	err = errno;
	pc_check(err == EDOM, "memcmp() of equal arrays changed errno to %s", pc_errname(err));
	errno = EDOM;
	(void)f_memcmp(s, t, 3);
	err = errno;
	pc_check(err == EDOM, "memcmp() of different arrays changed errno to %s", pc_errname(err));
	pc_pass();
}
