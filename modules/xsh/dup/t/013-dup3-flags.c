/*
 * dup3() takes FD_CLOEXEC from its flag argument only.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dup.013
 * MUTANT    dup.dup3_cloexec_ignored
 * MUTANT    dup.dup3_keeps_cloexec
 * VARIANT   dup.hidden_fds
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int a1, a2, b, c, f;

	pc_begin("XSH.dup.013");
	pc_make_file("f", "x");
	/* Each source and old target has the opposite of the flag wanted. */
	a1 = open("f", O_RDONLY | O_CLOEXEC);
	b = open("f", O_RDONLY | O_CLOEXEC);
	a2 = open("f", O_RDONLY);
	c = open("f", O_RDONLY);
	pc_check(a1 >= 0 && b >= 0 && a2 >= 0 && c >= 0, "cannot open f");
	pc_test();
	pc_check(dup3(a1, b, 0) == b, "dup3(a1, b, 0) failed");
	f = fcntl(b, F_GETFD);
	pc_check(f != -1 && !(f & FD_CLOEXEC), "dup3(..., 0): FD_CLOEXEC set");
	pc_check(dup3(a2, c, O_CLOEXEC) == c, "dup3(a2, c, O_CLOEXEC) failed");
	f = fcntl(c, F_GETFD);
	pc_check(f != -1 && (f & FD_CLOEXEC), "dup3(..., O_CLOEXEC): FD_CLOEXEC clear");
	pc_pass();
}
