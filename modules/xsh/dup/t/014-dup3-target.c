/*
 * dup3() replaces fildes2 with a duplicate of fildes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dup.014
 * MUTANT    dup.dup3_as_dup
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int a, b, r;
	char c;

	pc_begin("XSH.dup.014");
	pc_make_file("a", "AB");
	pc_make_file("b", "X");
	a = open("a", O_RDONLY);
	b = open("b", O_RDONLY);
	pc_check(a >= 0 && b >= 0, "cannot open the files");
	pc_test();
	r = dup3(a, b, 0);
	pc_check(r == b, "dup3(a, b, 0) returned %d, expected %d", r, b);
	pc_check(read(a, &c, 1) == 1 && c == 'A', "read on fildes failed");
	pc_check(read(b, &c, 1) == 1, "read on fildes2 failed");
	pc_check(c == 'B', "fildes2 read '%c', expected 'B' (shared offset in file a)", c);
	pc_pass();
}
