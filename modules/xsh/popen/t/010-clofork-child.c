/*
 * The command's standard output has FD_CLOFORK clear.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.popen.010
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
	pc_begin("XSH.popen.010");
#ifndef FD_CLOFORK
	pc_unresolved("<fcntl.h> does not define FD_CLOFORK");
#else
	char line[64];
	FILE *f;

	guard(20);
	pc_make_file("data", "hi\n");
	pc_test();
	errno = 0;
	/* cat is not the last command, so the shell starts it by fork */
	f = popen("cat data; true", "r");
	pc_check(f != NULL, "popen() failed: %s", pc_errname(errno));
	pc_check(fgets(line, sizeof line, f) != NULL, "nothing could be read: the forked utility lost the command's standard output");
	pc_check(strcmp(line, "hi\n") == 0, "the stream yielded \"%s\", expected \"hi\\n\"", line);
	finish(f);
	pc_pass();
#endif
}
