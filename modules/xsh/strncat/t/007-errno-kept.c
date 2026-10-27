/*
 * strncat: errno kept.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncat.007
 */
#include "pc.h"
#include <string.h>
#include <errno.h>
static char *(*volatile f_strncat)(char *restrict, const char *restrict, size_t) = strncat;

int main(void)
{
	int err;
	char d[16];
	pc_begin("XSH.strncat.007");
	pc_test();
	errno = EDOM;
	d[0] = 0; (void)f_strncat(d, "abc", 2);
	err = errno;
	pc_check(err == EDOM, "strncat() changed errno to %s", pc_errname(err));
	pc_pass();
}
