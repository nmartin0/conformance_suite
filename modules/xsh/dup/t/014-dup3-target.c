/*
 * dup3() replaces fildes2 with a duplicate of fildes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dup.014
 * MUTANT    dup.dup3_as_dup
 * MUTANT    dup.dup2_leaks_old
 * VARIANT   dup.hidden_fds
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int a, b, p[2], r, fl;
	ssize_t n;
	char c;

	pc_begin("XSH.dup.014");
	pc_make_file("a", "AB");
	a = open("a", O_RDONLY);
	pc_check(a >= 0, "cannot open a");
	pc_check(pipe(p) == 0, "pipe failed");
	b = p[1];
	fl = fcntl(p[0], F_GETFL);
	pc_check(fl != -1 && fcntl(p[0], F_SETFL, fl | O_NONBLOCK) == 0,
	    "cannot set O_NONBLOCK");
	pc_test();
	r = dup3(a, b, 0);
	pc_check(r == b, "dup3(a, b, 0) returned %d, expected %d", r, b);
	pc_check(read(a, &c, 1) == 1 && c == 'A', "read on fildes failed");
	pc_check(read(b, &c, 1) == 1, "read on fildes2 failed");
	pc_check(c == 'B', "fildes2 read '%c', expected 'B' (shared offset in file a)", c);
	n = read(p[0], &c, 1);
	pc_check(n == 0, "the old fildes2 (pipe write end) was not closed: read gave %ld (%s)",
	    (long)n, n < 0 ? pc_errname(errno) : "-");
	pc_pass();
}
