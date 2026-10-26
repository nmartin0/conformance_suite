/*
 * A read error before any conversion returns EOF and sets errno and the error indicator.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fscanf.046
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
	int x = -1, r, e, wfd;

	pc_begin("XSH.fscanf.046");
	pc_make_file("f", "12");
	f = fopen("f", "r");
	pc_check(f != NULL, "fopen(f) failed: %s", pc_errname(errno));
	wfd = open("w", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	pc_check(wfd >= 0, "open(w) failed: %s", pc_errname(errno));
	/* the stream's descriptor is now open for writing only */
	pc_check(dup2(wfd, fileno(f)) == fileno(f), "dup2() failed: %s", pc_errname(errno));
	(void)close(wfd);
	pc_test();
	errno = 0;
	r = fscanf(f, "%d", &x);
	e = errno;
	pc_check(r == EOF, "fscanf() returned %d, expected EOF", r);
	pc_check(e == EBADF, "errno is %s, expected EBADF", pc_errname(e));
	pc_check(ferror(f), "the error indicator is not set");
	pc_pass();
}
