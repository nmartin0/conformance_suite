/*
 * vscanf() reads from standard input.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.vfscanf.002
 * MUTANT    vfscanf.own_buffer
 * MUTANT    vfscanf.item_next_lost
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

static int call(const char *fmt, ...)
{
	va_list ap;
	int r;

	va_start(ap, fmt);
	r = vscanf(fmt, ap);
	va_end(ap);
	return r;
}

int main(void)
{
	int a = -1, r, c, fd;

	pc_begin("XSH.vfscanf.002");
	pc_make_file("in", "42 x");
	fd = open("in", O_RDONLY);
	pc_check(fd >= 0, "open(in) failed: %s", pc_errname(errno));
	pc_check(dup2(fd, 0) == 0, "dup2() onto 0 failed: %s", pc_errname(errno));
	(void)close(fd);
	pc_test();
	r = call("%d", &a);
	pc_check(r == 1, "vscanf() returned %d, expected 1", r);
	pc_check(a == 42, "vscanf() stored %d, expected 42", a);
	c = getchar();
	pc_check(c == ' ', "the next getchar() returned %#x, expected the space", (unsigned)c);
	pc_pass();
}
