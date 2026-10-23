/*
 * O_NONBLOCK does not matter when data is there.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.read.023
 * MUTANT    read.nonblock_data_eagain
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	int p[2], fl;
	char c;

	pc_begin("XSH.read.023");
	pc_check(pipe(p) == 0, "pipe failed");
	pc_check(write(p[1], "d", 1) == 1, "write failed");
	fl = fcntl(p[0], F_GETFL);
	pc_check(fl != -1 && fcntl(p[0], F_SETFL, fl | O_NONBLOCK) == 0, "cannot set O_NONBLOCK");
	pc_test();
	pc_check(read(p[0], &c, 1) == 1 && c == 'd', "non-blocking read() of a pipe with data failed");
	pc_pass();
}
