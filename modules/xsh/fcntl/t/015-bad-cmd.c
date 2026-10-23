/*
 * An undefined cmd gives EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fcntl.015
 * MUTANT    fcntl.badcmd_ok
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;

	pc_begin("XSH.fcntl.015");
	pc_make_file("f", "x");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_test();
	pc_expect_err(fcntl(fd, 0x7ffffff0, 0), EINVAL, "fcntl(fd, <undefined cmd>)");
	pc_pass();
}
