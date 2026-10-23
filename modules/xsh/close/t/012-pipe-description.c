/*
 * An open file description lives until its last descriptor is closed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.close.012
 * MUTANT    close.noop
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int p[2], d, fl;
	ssize_t n;
	char c;

	pc_begin("XSH.close.012");
	pc_check(pipe(p) == 0, "pipe failed");
	d = dup(p[1]);
	pc_check(d >= 0, "dup failed");
	fl = fcntl(p[0], F_GETFL);
	pc_check(fl != -1 && fcntl(p[0], F_SETFL, fl | O_NONBLOCK) == 0,
	    "cannot set O_NONBLOCK");
	pc_test();
	pc_expect_ok(close(p[1]), "close(write end)");
	n = read(p[0], &c, 1);
	pc_check(n == -1 && (errno == EAGAIN || errno == EWOULDBLOCK),
	    "write end freed while a duplicate was open: read gave %ld (%s)",
	    (long)n, n < 0 ? pc_errname(errno) : "-");
	pc_expect_ok(close(d), "close(last write descriptor)");
	n = read(p[0], &c, 1);
	pc_check(n == 0, "read() gave %ld (%s) after the last writer closed, expected 0",
	    (long)n, n < 0 ? pc_errname(errno) : "-");
	pc_pass();
}
