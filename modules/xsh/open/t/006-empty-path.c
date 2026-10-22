/*
 * An empty path fails with ENOENT.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.006
 * MUTANT    open.empty_is_dot
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>

int main(void)
{
	int fd;

	pc_begin("XSH.open.006");
	pc_test();
	fd = open("", O_RDONLY);
	pc_expect_err(fd, ENOENT, "open(\"\", O_RDONLY)");
	pc_pass();
}
