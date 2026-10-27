/*
 * posix_fadvise() with an unknown advice gives EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_fadvise.004
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
	int fd, r, v;

	pc_begin("XSH.posix_fadvise.004");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDONLY);
	if (fd < 0)
		pc_unresolved("open(f) failed: %s", pc_errname(errno));
	v = bad_advice();
	pc_test();
	r = posix_fadvise(fd, 0, 0, v);
	pc_check(r == EINVAL, "posix_fadvise() with advice %d returned %s", v, pc_errname(r));
	pc_pass();
}
