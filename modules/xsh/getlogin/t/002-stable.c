/*
 * Repeated getlogin() calls agree.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getlogin.002
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
	char first[512];
	const char *p;
	int i, had;

	pc_begin("XSH.getlogin.002");
	pc_test();
	p = getlogin();
	had = p != NULL;
	if (had)
		(void)snprintf(first, sizeof first, "%s", p);
	for (i = 0; i < 3; i++) {
		p = getlogin();
		pc_check((p != NULL) == had, "getlogin() returned %s on call %d, but %s on the first", p ? "a name" : "null", i + 2, had ? "a name" : "null");
		if (had)
			pc_check(strcmp(first, p) == 0, "getlogin() returned \"%s\" on call %d, \"%s\" on the first", p, i + 2, first);
	}
	pc_pass();
}
