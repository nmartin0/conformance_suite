/*
 * vfscanf() reports a read error as fscanf() does.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.vfscanf.005
 * MUTANT    vfscanf.error_returns_zero
 * MUTANT    vfscanf.error_clears_indicator
 * MUTANT    vfscanf.reads_stdin
 * VARIANT   setvbuf.tiny_default
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

static int call(FILE *f, const char *fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vfscanf(f, fmt, ap);
	va_end(ap);
	return r;
}

int main(void)
{
	FILE *f;
	int x = -1, r, e, wfd;

	pc_begin("XSH.vfscanf.005");
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
	r = call(f, "%d", &x);
	e = errno;
	pc_check(r == EOF, "vfscanf() returned %d, expected EOF", r);
	pc_check(e == EBADF, "errno is %s, expected EBADF", pc_errname(e));
	pc_check(ferror(f), "the error indicator is not set");
	pc_pass();
}
