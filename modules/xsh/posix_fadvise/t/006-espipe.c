/*
 * posix_fadvise() on a pipe or FIFO gives ESPIPE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_fadvise.006
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <poll.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static const int advs[] = { POSIX_FADV_NORMAL, POSIX_FADV_SEQUENTIAL, POSIX_FADV_RANDOM, POSIX_FADV_WILLNEED, POSIX_FADV_DONTNEED, POSIX_FADV_NOREUSE };
static const char *const advname[] = { "NORMAL", "SEQUENTIAL", "RANDOM", "WILLNEED", "DONTNEED", "NOREUSE" };

/* A value that is none of the six advice constants. */
static int bad_advice(void)
{
	int v, i, ok;

	for (v = 0;; v++) {
		ok = 1;
		for (i = 0; i < 6; i++)
			if (advs[i] == v)
				ok = 0;
		if (ok)
			return v;
	}
}


int main(void)
{
	int p[2], rd, wr, r;

	pc_begin("XSH.posix_fadvise.006");
	if (pipe(p) != 0)
		pc_unresolved("pipe failed: %s", pc_errname(errno));
	if (mkfifo("ff", 0600) != 0)
		pc_unresolved("mkfifo failed: %s", pc_errname(errno));
	rd = open("ff", O_RDONLY | O_NONBLOCK);
	if (rd < 0)
		pc_unresolved("open(ff) for reading failed: %s", pc_errname(errno));
	wr = open("ff", O_WRONLY | O_NONBLOCK);
	if (wr < 0)
		pc_unresolved("open(ff) for writing failed: %s", pc_errname(errno));
	pc_test();
	r = posix_fadvise(p[0], 0, 0, POSIX_FADV_NORMAL);
	pc_check(r == ESPIPE, "read end of a pipe: returned %s", pc_errname(r));
	r = posix_fadvise(p[1], 0, 0, POSIX_FADV_NORMAL);
	pc_check(r == ESPIPE, "write end of a pipe: returned %s", pc_errname(r));
	r = posix_fadvise(rd, 0, 0, POSIX_FADV_NORMAL);
	pc_check(r == ESPIPE, "FIFO (read side): returned %s", pc_errname(r));
	r = posix_fadvise(wr, 0, 0, POSIX_FADV_NORMAL);
	pc_check(r == ESPIPE, "FIFO (write side): returned %s", pc_errname(r));
	pc_pass();
}
