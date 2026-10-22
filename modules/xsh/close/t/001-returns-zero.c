/*
 * close() of an open descriptor returns 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.close.001
 * MUTANT    close.fail_valid
 */
#include "pc.h"
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;

	pc_begin("XSH.close.001");
	pc_make_file("f", "x");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "open");
	pc_test();
	pc_check(close(fd) == 0, "close returned non-zero");
	pc_pass();
}
