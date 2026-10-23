/*
 * dup2() onto an open descriptor replaces it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dup.007
 * MUTANT    dup.dup2_picks_lowest
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int a, b, r;
	char c;

	pc_begin("XSH.dup.007");
	pc_make_file("a", "A");
	pc_make_file("b", "B");
	a = open("a", O_RDONLY);
	b = open("b", O_RDONLY);
	pc_check(a >= 0 && b >= 0, "cannot open the files");
	pc_test();
	r = dup2(a, b);
	pc_check(r == b, "dup2(a, b) returned %d, expected %d", r, b);
	pc_check(read(b, &c, 1) == 1, "read on fildes2 failed");
	pc_check(c == 'A', "fildes2 still reads file b");
	pc_pass();
}
