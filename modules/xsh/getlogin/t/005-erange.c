/*
 * getlogin_r() with too short a buffer fails with ERANGE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getlogin.005
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
	char big[256], small[256], exact[256];
	size_t n;
	int r;

	pc_begin("XSH.getlogin.005");
	if (getlogin_r(big, sizeof big) != 0)
		pc_unresolved("getlogin_r() found no login name here");
	n = strnlen(big, sizeof big);
	if (n == 0 || n >= sizeof big)
		pc_unresolved("the login name has an unexpected length");
	memset(small, 'Z', sizeof small);
	pc_test();
	r = getlogin_r(small, n);	/* room for the name but not for its null byte */
	pc_check(r == ERANGE, "getlogin_r() with namesize %d (the name's length) returned %d, expected ERANGE (%d)", (int)n, r, ERANGE);
	r = getlogin_r(exact, n + 1);
	pc_check(r == 0, "getlogin_r() with namesize %d (name and null byte) returned %d, expected 0", (int)n + 1, r);
	pc_check(strcmp(exact, big) == 0, "getlogin_r() stored \"%s\" in an exact-fit buffer, expected \"%s\"", exact, big);
	pc_pass();
}
