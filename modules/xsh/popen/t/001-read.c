/*
 * popen("r") yields the command's output.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.popen.001
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

/* A call that blocks must fail the test, not hang it. */
static void on_guard(int sig)
{
	(void)sig;
	pc_fail("blocked: the call under test did not finish in time");
}

static void guard(unsigned secs)
{
	struct sigaction sa;

	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_guard;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	alarm(secs);
}
static void finish(FILE *f)
{
	if (pclose(f) == -1)
		pc_unresolved("pclose() failed: %s", pc_errname(errno));
}

int main(void)
{
	char line[64];
	FILE *f;

	pc_begin("XSH.popen.001");
	guard(20);
	pc_test();
	errno = 0;
	f = popen("x=ab; echo ${x}c", "r");
	pc_check(f != NULL, "popen() failed: %s", pc_errname(errno));
	pc_check(fgets(line, sizeof line, f) != NULL, "nothing could be read from the stream");
	pc_check(strcmp(line, "abc\n") == 0, "the stream yielded \"%s\", expected \"abc\\n\"", line);
	pc_check(fgetc(f) == EOF, "more data than the command's output followed");
	finish(f);
	pc_pass();
}
