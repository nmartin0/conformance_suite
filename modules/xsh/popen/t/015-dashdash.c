/*
 * A command word starting with - or + is a command.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.popen.015
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

/*
 * Creates the executable files -pcdash and +pcplus in the working directory
 * (shell scripts without an interpreter line: the shell runs a file the
 * system cannot execute itself, XCU 2.9.1.6) and puts the directory first in PATH.
 */
static void make_dash_commands(const char *body_dash, const char *body_plus)
{
	char cwd[PATH_MAX], path[PATH_MAX * 2 + 8];
	const char *old = getenv("PATH");

	pc_make_file("-pcdash", body_dash);
	pc_make_file("+pcplus", body_plus);
	if (chmod("-pcdash", 0755) != 0 || chmod("+pcplus", 0755) != 0)
		pc_unresolved("chmod failed: %s", pc_errname(errno));
	if (getcwd(cwd, sizeof cwd) == NULL)
		pc_unresolved("getcwd failed: %s", pc_errname(errno));
	(void)snprintf(path, sizeof path, "%s%s%s", cwd, old && *old ? ":" : "", old ? old : "");
	if (setenv("PATH", path, 1) != 0)
		pc_unresolved("setenv failed: %s", pc_errname(errno));
}
static void finish(FILE *f)
{
	if (pclose(f) == -1)
		pc_unresolved("pclose() failed: %s", pc_errname(errno));
}

static void expect(const char *cmd, const char *line)
{
	char b[64];
	FILE *f;

	errno = 0;
	f = popen(cmd, "r");
	pc_check(f != NULL, "popen(\"%s\") failed: %s", cmd, pc_errname(errno));
	pc_check(fgets(b, sizeof b, f) != NULL, "popen(\"%s\") yielded nothing", cmd);
	pc_check(strcmp(b, line) == 0, "popen(\"%s\") yielded \"%s\", expected \"%s\"", cmd, b, line);
	finish(f);
}

int main(void)
{
	pc_begin("XSH.popen.015");
	guard(20);
	make_dash_commands("echo dash\n", "echo plus\n");
	pc_test();
	expect("-pcdash", "dash\n");
	expect("+pcplus", "plus\n");
	pc_pass();
}
