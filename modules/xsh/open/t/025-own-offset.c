/*
 * Two opens of one file have independent offsets.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.025
 * MUTANT    open.reuses_description
 * VARIANT   open.accmode_bits
 */
#include "pc.h"
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int a, b;
	char c;

	pc_begin("XSH.open.025");
	pc_make_file("f", "abc");
	pc_test();
	a = open("f", O_RDONLY);
	pc_expect_ok(a, "first open()");
	b = open("f", O_RDONLY);
	pc_expect_ok(b, "second open()");
	pc_check(read(a, &c, 1) == 1 && c == 'a', "read on the first failed");
	pc_check(read(a, &c, 1) == 1 && c == 'b', "read on the first failed");
	pc_check(read(b, &c, 1) == 1, "read on the second failed");
	pc_check(c == 'a', "second descriptor read '%c': it shares the first's offset", c);
	pc_pass();
}
