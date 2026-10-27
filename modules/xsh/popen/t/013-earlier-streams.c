/*
 * A later command does not hold an earlier stream.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.popen.013
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

/* The file's contents (up to 255 bytes) as a string; "" if it cannot be read. */
static const char *slurp_file(const char *path)
{
	static char b[256];
	int f = open(path, O_RDONLY);
	ssize_t n;

	b[0] = '\0';
	if (f < 0)
		return b;
	n = read(f, b, sizeof b - 1);
	(void)close(f);
	if (n > 0)
		b[n] = '\0';
	else
		b[0] = '\0';
	return b;
}
static void finish(FILE *f)
{
	if (pclose(f) == -1)
		pc_unresolved("pclose() failed: %s", pc_errname(errno));
}

int main(void)
{
	FILE *a, *b;

	pc_begin("XSH.popen.013");
	guard(20);
	pc_test();
	errno = 0;
	a = popen("cat > out", "w");
	pc_check(a != NULL, "first popen() failed: %s", pc_errname(errno));
	errno = 0;
	b = popen("cat > /dev/null", "w");
	pc_check(b != NULL, "second popen() failed: %s", pc_errname(errno));
	pc_check(fputs("data\n", a) >= 0, "writing to the first stream failed");
	/* The first command sees end-of-file only if the second one holds no copy of the pipe. */
	pc_check(pclose(a) != -1, "pclose() of the first stream failed: %s", pc_errname(errno));
	pc_check(strcmp(slurp_file("out"), "data\n") == 0, "the first command received \"%s\", expected \"data\\n\"", slurp_file("out"));
	finish(b);
	pc_pass();
}
