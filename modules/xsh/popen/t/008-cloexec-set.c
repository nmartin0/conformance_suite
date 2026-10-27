/*
 * With 'e', FD_CLOEXEC is set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.popen.008
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
	char b[8];
	FILE *f;
	int flags;

	pc_begin("XSH.popen.008");
	guard(20);
	pc_test();
	errno = 0;
	f = popen("echo hi", "re");
	pc_check(f != NULL, "popen(..., \"re\") failed: %s", pc_errname(errno));
	flags = fcntl(fileno(f), F_GETFD);
	pc_check(flags != -1, "fcntl(F_GETFD) failed: %s", pc_errname(errno));
	pc_check(flags & FD_CLOEXEC, "FD_CLOEXEC is clear on the descriptor under a \"re\" stream");
	pc_check(fgets(b, sizeof b, f) != NULL && strcmp(b, "hi\n") == 0, "the \"re\" stream did not yield the command's output");
	finish(f);
	errno = 0;
	f = popen("cat > out", "we");
	pc_check(f != NULL, "popen(..., \"we\") failed: %s", pc_errname(errno));
	flags = fcntl(fileno(f), F_GETFD);
	pc_check(flags != -1, "fcntl(F_GETFD) failed: %s", pc_errname(errno));
	pc_check(flags & FD_CLOEXEC, "FD_CLOEXEC is clear on the descriptor under a \"we\" stream");
	pc_check(fputs("data\n", f) >= 0, "writing to the \"we\" stream failed");
	finish(f);
	pc_check(strcmp(slurp_file("out"), "data\n") == 0, "the command received \"%s\" through the \"we\" stream", slurp_file("out"));
	pc_pass();
}
