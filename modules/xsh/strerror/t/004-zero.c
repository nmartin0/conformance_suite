/*
 * strerror(0) leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strerror.004
 * MUTANT    strerror.zero_einval
 * VARIANT   strerror.zero_empty
 */
#include "pc.h"
#include <errno.h>
#include <string.h>

int main(void)
{
	const char *s;
	int e;

	pc_begin("XSH.strerror.004");
	pc_test();
	errno = 0;
	s = strerror(0);
	e = errno;
	pc_check(s != NULL, "strerror(0) returned a null pointer");
	pc_check(e == 0, "strerror(0) changed errno from 0 to %s", pc_errname(e));
	pc_pass();
}
