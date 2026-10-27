/*
 * uname() gives the same answer each time.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.uname.002
 */
#include "pc.h"
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <unistd.h>

static int same(const struct utsname *a, const struct utsname *b)
{
	return strcmp(a->sysname, b->sysname) == 0 && strcmp(a->release, b->release) == 0 &&
	    strcmp(a->version, b->version) == 0 && strcmp(a->machine, b->machine) == 0;
}

int main(void)
{
	struct utsname a, b;
	pid_t c;
	int st;

	pc_begin("XSH.uname.002");
	pc_test();
	pc_check(uname(&a) >= 0 && uname(&b) >= 0, "uname() failed");
	pc_check(same(&a, &b), "two calls gave different identification");
	c = fork();
	if (c == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (c == 0) {
		struct utsname k;

		alarm(20);
		_exit(uname(&k) >= 0 && same(&a, &k) ? 0 : 1);
	}
	while (waitpid(c, &st, 0) == -1)
		;
	pc_check(WIFEXITED(st) && WEXITSTATUS(st) == 0, "a child saw different identification (status %#x)", st);
	pc_pass();
}
