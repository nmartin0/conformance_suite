/*
 * getgid() does not modify errno.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getgid.003
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

int main(void)
{
	int e;

	pc_begin("XSH.getgid.003");
	pc_test();
	errno = EINTR;
	(void)getgid();
	e = errno;
	pc_check(e == EINTR, "getgid() changed errno from EINTR to %s", pc_errname(e));
	errno = 0;
	(void)getgid();
	e = errno;
	pc_check(e == 0, "getgid() changed errno from 0 to %s", pc_errname(e));
	pc_pass();
}
