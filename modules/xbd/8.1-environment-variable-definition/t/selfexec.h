/*
 * selfexec.h - run the test program itself as a new process image with a
 * chosen environment, and collect what that image reports.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * The test defines static void image(int fd), which runs in the new image
 * (argv[1] is "img", argv[2] the report descriptor) and writes its report
 * to fd; SE_MAIN supplies main().
 */
#ifndef SELFEXEC_H
#define SELFEXEC_H

#include "pc.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

static char *se_self;

static void image(int fd);

/* Run the image with envp as its whole environment; its report is left in rep (NUL-terminated). */
static void se_run(char *const envp[], char *rep, size_t n)
{
	int p[2], st;
	pid_t k;
	size_t got = 0;
	ssize_t r;
	char fdarg[16];
	char *av[4];

	if (se_self == NULL || strchr(se_self, '/') == NULL)
		pc_unresolved("the program was not started by a pathname, so it cannot run itself");
	if (pipe(p) != 0)
		pc_unresolved("pipe failed: %s", pc_errname(errno));
	k = fork();
	if (k == -1)
		pc_unresolved("fork failed: %s", pc_errname(errno));
	if (k == 0) {
		(void)close(p[0]);
		(void)snprintf(fdarg, sizeof fdarg, "%d", p[1]);
		av[0] = se_self;
		av[1] = (char *)"img";
		av[2] = fdarg;
		av[3] = NULL;
		(void)execve(se_self, av, envp);
		_exit(99);
	}
	(void)close(p[1]);
	(void)alarm(60);
	while (got < n - 1 && (r = read(p[0], rep + got, n - 1 - got)) > 0)
		got += (size_t)r;
	(void)alarm(0);
	rep[got] = '\0';
	(void)close(p[0]);
	if (waitpid(k, &st, 0) != k)
		pc_unresolved("waitpid failed: %s", pc_errname(errno));
	if (WIFEXITED(st) && WEXITSTATUS(st) == 99)
		pc_unresolved("the program could not run itself with the chosen environment");
}

#define SE_MAIN \
	int main(int argc, char **argv) \
	{ \
		if (argc == 3 && strcmp(argv[1], "img") == 0) { \
			image(atoi(argv[2])); \
			return 0; \
		} \
		se_self = argv[0]; \
		return run_test(); \
	}

#endif
