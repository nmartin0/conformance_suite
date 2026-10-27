/*
 * getlogin_r() returns 0 or an error number.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getlogin.003
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
	char buf[256];
	size_t n;
	int r;

	pc_begin("XSH.getlogin.003");
	pc_test();
	memset(buf, 'Z', sizeof buf);
	r = getlogin_r(buf, sizeof buf);
	pc_check(r >= 0, "getlogin_r() returned %d: the error number is returned, never -1", r);
	if (r == 0) {
		n = strnlen(buf, sizeof buf);
		pc_check(n < sizeof buf, "the name is not null-terminated within the buffer");
		pc_check(n > 0, "getlogin_r() stored an empty name");
		{
			long max = sysconf(_SC_LOGIN_NAME_MAX);

			pc_check(max <= 0 || n <= (size_t)max, "the name is longer than {LOGIN_NAME_MAX}");
		}
	}
	pc_pass();
}
