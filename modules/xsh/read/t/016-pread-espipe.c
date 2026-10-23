/*
 * pread() on a pipe gives ESPIPE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.read.016
 * MUTANT    read.pread_espipe_as_einval
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	int p[2];
	char c;

	pc_begin("XSH.read.016");
	pc_check(pipe(p) == 0, "pipe failed");
	pc_check(write(p[1], "x", 1) == 1, "write failed");
	pc_test();
	pc_expect_err(pread(p[0], &c, 1, 0), ESPIPE, "pread() on a pipe");
	pc_pass();
}
