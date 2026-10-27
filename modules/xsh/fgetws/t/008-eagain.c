/*
 * fgetws() on an empty non-blocking pipe fails with EAGAIN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fgetws.008
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <locale.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

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
	pc_begin("XSH.fgetws.008");
	{
		wchar_t ws[8];
		wchar_t *r;
		FILE *f;
		int p[2], e;

		pc_check(pipe(p) == 0, "pipe failed: %s", pc_errname(errno));
		nonblock(p[0]);
		pc_test();
		f = fdopen(p[0], "r");
		NEED(f != NULL, "fdopen() failed: %s", pc_errname(errno));
		errno = 0;
		r = fgetws(ws, 8, f);
		e = errno;
		pc_check(r == NULL, "fgetws() on an empty non-blocking pipe returned %p, expected a null pointer", (void *)r);
		pc_check(e == EAGAIN, "fgetws() set errno %s, expected EAGAIN", pc_errname(e));
		pc_check(ferror(f) != 0, "the stream's error indicator is not set");
	}
	pc_pass();
}
