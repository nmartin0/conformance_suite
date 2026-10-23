/*
 * A small non-blocking pipe write with room is complete.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.013
 * MUTANT    write.nonblock_short
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	int p[2], fl;
	char *buf;

	pc_begin("XSH.write.013");
	pc_check(pipe(p) == 0, "pipe failed");
	fl = fcntl(p[1], F_GETFL);
	pc_check(fl != -1 && fcntl(p[1], F_SETFL, fl | O_NONBLOCK) == 0, "cannot set O_NONBLOCK");
	buf = calloc(1, PIPE_BUF);
	pc_check(buf != NULL, "out of memory");
	pc_test();
	pc_check(write(p[1], buf, PIPE_BUF) == PIPE_BUF, "non-blocking write of {PIPE_BUF} bytes to an empty pipe was short");
	pc_pass();
}
