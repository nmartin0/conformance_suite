/*
 * A caught signal does not end system() early.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.system.018
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

static volatile sig_atomic_t fired;

static void on_alarm(int sig)
{
	(void)sig;
	fired = 1;
}

int main(void)
{
	struct sigaction sa;
	int r;

	pc_begin("XSH.system.018");
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_alarm;	/* no SA_RESTART */
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	pc_test();
	alarm(1);
	r = system("sleep 2; echo done > f");
	alarm(0);
	if (!fired)
		pc_unresolved("SIGALRM was not delivered to the caller during system()");
	pc_check(r != -1, "system() returned -1 (%s) after the signal", pc_errname(errno));
	pc_check(WIFEXITED(r) && WEXITSTATUS(r) == 0, "status %#x", r);
	pc_check(strcmp(slurp_file("f"), "done\n") == 0, "system() returned before the command had finished (file holds \"%s\")", slurp_file("f"));
	pc_pass();
}
