/*
 * dup2() onto an open descriptor replaces it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dup.007
 * MUTANT    dup.dup2_picks_lowest
 * MUTANT    dup.dup2_leaks_old
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

	pc_begin("XSH.dup.007");
	pc_make_file("a", "A");
	a = open("a", O_RDONLY);
	pc_check(a >= 0, "cannot open a");
	/* fildes2 is the only write end of a pipe: its close shows as EOF. */
	pc_check(pipe(p) == 0, "pipe failed");
	b = p[1];
	fl = fcntl(p[0], F_GETFL);
	pc_check(fl != -1 && fcntl(p[0], F_SETFL, fl | O_NONBLOCK) == 0,
	    "cannot set O_NONBLOCK");
	pc_test();
	r = dup2(a, b);
	pc_check(r == b, "dup2(a, b) returned %d, expected %d", r, b);
	pc_check(read(b, &c, 1) == 1 && c == 'A', "fildes2 does not read file a");
	n = read(p[0], &c, 1);
	pc_check(n == 0, "the old fildes2 (pipe write end) was not closed: read gave %ld (%s)",
	    (long)n, n < 0 ? pc_errname(errno) : "-");
	pc_pass();
}
