/*
 * getlogin() and getlogin_r() give the same name.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getlogin.004
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
	char buf[256], first[512];
	const char *p;
	int r;

	pc_begin("XSH.getlogin.004");
	p = getlogin();
	if (p == NULL)
		pc_unresolved("getlogin() found no login name here");
	(void)snprintf(first, sizeof first, "%s", p);
	pc_test();
	r = getlogin_r(buf, sizeof buf);
	if (r != 0)
		pc_unresolved("getlogin_r() gave error %d though getlogin() found a name", r);	/* both may fail at different points; nothing to compare */
	pc_check(strcmp(buf, first) == 0, "getlogin_r() stored \"%s\", getlogin() returned \"%s\"", buf, first);
	pc_pass();
}
