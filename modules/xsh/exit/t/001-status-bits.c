/*
 * exit() status: the low 8 bits through waitpid(); exit does not return.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.exit.001
 * MUTANT    exit.status_wrong
 * MUTANT    exit.returns
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* Waits for pid and returns its raw status; anything odd is UNRESOLVED. */
static int reap(pid_t pid)
{
	int st;

	if (waitpid(pid, &st, 0) != pid)
		pc_unresolved("waitpid failed");
	if (WIFEXITED(st) && WEXITSTATUS(st) == 99)
		pc_unresolved("the child could not set up");
	return st;
}

static pid_t spawn(void)
{
	pid_t r = fork();

	if (r == -1)
		pc_unresolved("fork failed");
	return r;
}

/* The child's exit handlers record their names here, in call order. */
static int trace_fd = -1;

static void mark(const char *s)
{
	(void)write(trace_fd, s, strlen(s));
}

/* Reads what the child recorded, up to end of file. */
static void collect(int fd, char *buf, size_t size)
{
	size_t n = 0;
	ssize_t r;

	while (n < size - 1 && (r = read(fd, buf + n, size - 1 - n)) > 0)
		n += (size_t)r;
	buf[n] = '\0';
}

int main(void)
{
	int p[2], st;
	pid_t c;
	char buf[16];

	pc_begin("XSH.exit.001");
	pc_check(pipe(p) == 0, "pipe failed");
	pc_test();
	c = spawn();
	if (c == 0) {
		trace_fd = p[1];
		(void)close(p[0]);
		/* Through a volatile pointer: exit() is declared noreturn, so a
		 * direct call would let the compiler drop the code after it. */
		void (*volatile ex)(int) = exit;
		ex(0x1234);
		mark("r");	/* exit() returned */
		_exit(98);
	}
	(void)close(p[1]);
	st = reap(c);
	collect(p[0], buf, sizeof buf);
	pc_check(buf[0] == '\0' && !(WIFEXITED(st) && WEXITSTATUS(st) == 98),
	    "exit() returned to its caller");
	pc_check(WIFEXITED(st), "the child did not exit normally (%#x)", st);
	pc_check(WEXITSTATUS(st) == 0x34, "exit status %#x, expected 0x34", WEXITSTATUS(st));
	pc_pass();
}
