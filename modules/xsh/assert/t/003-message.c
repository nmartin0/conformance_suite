/*
 * The message of a failed assert().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.assert.003
 */
#include "pc.h"
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static void on_alarm(int sig)
{
	(void)sig;
	pc_fail("a child did not finish in time");
}

/*
 * With run 0 returns the line number of the assert() below without
 * executing it; with run 1 executes it. The assert() must stay exactly three
 * lines below the enum.
 */
static int marker_function_xyz(int run)
{
	int widget = 3;
	enum { assert_line = __LINE__ + 3 };
	if (!run)
		return assert_line;
	assert(widget == 4 && "marker text");
	return 0;
}

static size_t slurp(int fd, char *buf, size_t n)
{
	size_t got = 0;
	ssize_t k;

	while (got < n - 1 && (k = read(fd, buf + got, n - 1 - got)) != 0) {
		if (k > 0)
			got += (size_t)k;
		else if (errno != EINTR)
			break;
	}
	buf[got] = '\0';
	return got;
}

int main(void)
{
	char err[2048], out[256], num[32];
	int line;
	int pe[2], po[2], st;
	pid_t c;
	struct sigaction sa;

	pc_begin("XSH.assert.003");
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_alarm;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	alarm(30);
	pc_check(pipe(pe) == 0 && pipe(po) == 0, "pipe failed");
	line = marker_function_xyz(0);
	pc_test();
	c = fork();
	if (c == -1)
		pc_unresolved("fork failed");
	if (c == 0) {
		(void)close(pe[0]);
		(void)close(po[0]);
		(void)dup2(pe[1], 2);
		(void)dup2(po[1], 1);
		alarm(20);
		(void)marker_function_xyz(1);
		_exit(0);
	}
	(void)close(pe[1]);
	(void)close(po[1]);
	(void)slurp(pe[0], err, sizeof err);
	(void)slurp(po[0], out, sizeof out);
	(void)close(pe[0]);
	(void)close(po[0]);
	while (waitpid(c, &st, 0) == -1)
		;
	pc_check(WIFSIGNALED(st) && WTERMSIG(st) == SIGABRT, "the child did not die of SIGABRT (status %#x)", st);
	pc_check(out[0] == '\0', "something was written to standard output: %s", out);
	pc_check(strstr(err, "widget == 4") != NULL, "the message lacks the text of the expression: %s", err);
	pc_check(strstr(err, __FILE__) != NULL, "the message lacks the file name %s: %s", __FILE__, err);
	(void)snprintf(num, sizeof num, "%d", line);
	pc_check(strstr(err, num) != NULL, "the message lacks the line number %s: %s", num, err);
	pc_check(strstr(err, "marker_function_xyz") != NULL, "the message lacks the function name: %s", err);
	pc_pass();
}
