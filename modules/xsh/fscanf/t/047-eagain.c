/*
 * On an empty non-blocking pipe fscanf() fails with EAGAIN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.047
 * MUTANT    fscanf.error_returns_zero
 * MUTANT    fscanf.error_clears_indicator
 * MUTANT    fscanf.error_errno_zero
 * VARIANT   setvbuf.tiny_default
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

int main(void)
{
	FILE *f;
	int p[2], x = -1, r, e;

	pc_begin("XSH.fscanf.047");
	pc_check(pipe(p) == 0, "pipe() failed: %s", pc_errname(errno));
	pc_check(fcntl(p[0], F_SETFL, O_NONBLOCK) == 0, "fcntl(O_NONBLOCK) failed: %s", pc_errname(errno));
	f = fdopen(p[0], "r");
	pc_check(f != NULL, "fdopen() failed: %s", pc_errname(errno));
	pc_test();
	errno = 0;
	r = fscanf(f, "%d", &x);
	e = errno;
	pc_check(r == EOF, "fscanf() returned %d, expected EOF", r);
	pc_check(e == EAGAIN, "errno is %s, expected EAGAIN", pc_errname(e));
	pc_check(ferror(f), "the error indicator is not set");
	pc_pass();
}
