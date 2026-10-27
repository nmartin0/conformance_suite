/*
 * getchar() with the end-of-file indicator set returns EOF until it is cleared.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getchar.003
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* Opens path with flags and makes it descriptor target (setup). */
static void fd_onto(const char *path, int flags, int target)
{
	int fd = open(path, flags, 0666);

	pc_check(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	if (fd != target) {
		if (!(dup2(fd, target) == target))
			pc_unresolved("dup2 to %d failed: %s", target, pc_errname(errno));
		pc_check(close(fd) == 0, "close failed: %s", pc_errname(errno));
	}
}

/* Appends s to path through a separate open() (a precondition). */
static void append_other(const char *path, const char *s)
{
	int fd = open(path, O_WRONLY | O_APPEND);
	size_t n = strlen(s);

	NEED(fd >= 0, "open(%s) failed: %s", path, pc_errname(errno));
	NEED(write(fd, s, n) == (ssize_t)n, "write(%s) failed: %s", path, pc_errname(errno));
	NEED(close(fd) == 0, "close(%s) failed: %s", path, pc_errname(errno));
}

int main(void)
{
	pc_begin("XSH.getchar.003");
	{
		int c;

		pc_make_file("f", "ab");
		fd_onto("f", O_RDONLY, 0);
		pc_test();
		NEED(getchar() == 'a' && getchar() == 'b', "the first two reads of \"ab\" did not return 'a' and 'b'");
		NEED(getchar() == EOF && feof(stdin), "the third read did not reach the end of the file");
		append_other("f", "c");
		c = getchar();
		pc_check(c == EOF, "with the end-of-file indicator set, getchar() returned %d after the file grew, expected EOF", c);
		pc_check(feof(stdin) != 0, "the end-of-file indicator is no longer set");
		clearerr(stdin);
		c = getchar();
		pc_check(c == 'c', "after clearerr(), getchar() returned %d, expected 'c'", c);
	}
	pc_pass();
}
