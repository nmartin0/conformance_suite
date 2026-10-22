/*
 * A closed descriptor is returned by the next open().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.close.002
 * MUTANT    close.noop
 */
#include "pc.h"
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int a, b, fd;

	pc_begin("XSH.close.002");
	pc_make_file("f", "x");
	a = open("f", O_RDONLY);
	b = open("f", O_RDONLY);
	pc_check(a >= 0 && b > a, "open");
	pc_test();
	pc_expect_ok(close(a), "close");
	fd = open("f", O_RDONLY);
	pc_expect_ok(fd, "open after close");
	pc_check(fd == a, "open returned %d, expected the closed %d", fd, a);
	pc_pass();
}
