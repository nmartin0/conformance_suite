/*
 * Without O_CREAT, a missing file fails with ENOENT.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.005
 * MUTANT    open.enoent_as_eacces
 * MUTANT    open.always_creat
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>

int main(void)
{
	int fd;

	pc_begin("XSH.open.005");
	pc_test();
	fd = open("missing", O_RDONLY);
	pc_expect_err(fd, ENOENT, "open(\"missing\", O_RDONLY)");
	pc_pass();
}
