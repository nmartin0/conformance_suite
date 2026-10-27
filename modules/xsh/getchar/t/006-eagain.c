/*
 * getchar() on an empty non-blocking pipe fails with EAGAIN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getchar.006
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

/* Sets O_NONBLOCK on fd (setup). */
static void nonblock(int fd)
{
	int fl = fcntl(fd, F_GETFL);

	pc_check(fl != -1 && fcntl(fd, F_SETFL, fl | O_NONBLOCK) == 0, "cannot set O_NONBLOCK: %s", pc_errname(errno));
}

int main(void)
{
	pc_begin("XSH.getchar.006");
	{
		int p[2], r, e;

		pc_check(pipe(p) == 0, "pipe failed: %s", pc_errname(errno));
		nonblock(p[0]);
		if (p[0] != 0) {
			if (!(dup2(p[0], 0) == 0))
				pc_unresolved("dup2 to 0 failed: %s", pc_errname(errno));
			if (!(close(p[0]) == 0))
				pc_unresolved("close failed: %s", pc_errname(errno));
		}
		pc_test();
		errno = 0;
		r = getchar();
		e = errno;
		pc_check(r == EOF, "getchar() on an empty non-blocking pipe returned %d, expected EOF", r);
		pc_check(e == EAGAIN, "getchar() set errno %s, expected EAGAIN", pc_errname(e));
		pc_check(ferror(stdin) != 0, "the error indicator of stdin is not set");
	}
	pc_pass();
}
