/*
 * sysconf of the suggested buffer size returns -1 with errno unchanged, or a value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getgrnam.008
 */
#include "grdb.h"

#include <errno.h>
#include <unistd.h>

int main(void)
{
	long v;

	pc_begin("XSH.getgrnam.008");
	pc_test();
	errno = XSENTINEL;
	v = sysconf(_SC_GETGR_R_SIZE_MAX);
	pc_check(v >= -1, "returned %ld", v);
	if (v == -1)
		pc_check(errno == XSENTINEL, "returned -1 and changed errno to %s", pc_errname(errno));
	pc_pass();
}
