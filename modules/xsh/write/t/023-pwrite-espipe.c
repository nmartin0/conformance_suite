/*
 * pwrite() on a pipe gives ESPIPE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.023
 * MUTANT    write.pwrite_espipe_as_einval
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	int p[2];

	pc_begin("XSH.write.023");
	pc_check(pipe(p) == 0, "pipe failed");
	pc_test();
	pc_expect_err(pwrite(p[1], "x", 1, 0), ESPIPE, "pwrite() on a pipe");
	pc_pass();
}
