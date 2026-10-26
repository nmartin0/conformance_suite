/*
 * fgets() that meets EAGAIN after reading part of a line returns a null pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fgets.006
 * MUTANT    fgets.partial_ok
 * MUTANT    fgets.errno_lost
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
	pc_begin("XSH.fgets.006");
	{
		FILE *f;
		char s[16], *r;
		int p[2], e;

		if (!(pipe(p) == 0))
			pc_unresolved("pipe failed: %s", pc_errname(errno));
		pc_check(write(p[1], "ab", 2) == 2, "write to the pipe failed: %s", pc_errname(errno));
		nonblock(p[0]);
		pc_test();
		f = fdopen(p[0], "r");
		NEED(f != NULL, "fdopen() failed: %s", pc_errname(errno));
		errno = 0;
		r = fgets(s, 16, f);
		e = errno;
		pc_check(r == NULL, "fgets() returned s although the read after \"ab\" could not be done without waiting");
		pc_check(e == EAGAIN, "fgets() set errno %s, expected EAGAIN", pc_errname(e));
		pc_check(ferror(f) != 0, "the stream's error indicator is not set");
	}
	pc_pass();
}
