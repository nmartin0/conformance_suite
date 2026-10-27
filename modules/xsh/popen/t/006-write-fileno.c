/*
 * With "w", fileno() is the writable end.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.popen.006
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
	ssize_t n;
	int fd;
	FILE *f;

	pc_begin("XSH.popen.006");
	guard(20);
	pc_test();
	errno = 0;
	f = popen("cat > out", "w");
	pc_check(f != NULL, "popen() failed: %s", pc_errname(errno));
	fd = fileno(f);
	pc_check(fd >= 0, "fileno() failed");
	n = write(fd, "data\n", 5);
	pc_check(n == 5, "write() on the descriptor returned %ld (%s), expected 5", (long)n, pc_errname(errno));
	finish(f);
	pc_check(strcmp(slurp_file("out"), "data\n") == 0, "the command received \"%s\", expected \"data\\n\"", slurp_file("out"));
	pc_pass();
}
