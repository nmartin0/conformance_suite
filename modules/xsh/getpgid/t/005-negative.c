/*
 * getpgid(-1) fails with ESRCH or EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getpgid.005
 * MUTANT    getpgid.neg_self
 * MUTANT    getpgid.neg_eperm
 * VARIANT   getpgid.einval_neg
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
	pid_t r;
	int e;

	pc_begin("XSH.getpgid.005");
	pc_test();
	r = getpgid((pid_t)-1);
	e = errno;
	pc_check(r == -1, "getpgid(-1) returned %ld, expected -1", (long)r);
	pc_check(e == ESRCH || e == EINVAL, "getpgid(-1) failed with %s, expected ESRCH or EINVAL", pc_errname(e));
	pc_pass();
}
