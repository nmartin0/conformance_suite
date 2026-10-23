/*
 * An empty pipe with no writer reads as end of file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.read.006
 * MUTANT    read.pipe_eof_eagain
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

	pc_begin("XSH.read.006");
	pc_check(pipe(p) == 0, "pipe failed");
	pc_check(close(p[1]) == 0, "close of the write end failed");
	pc_test();
	pc_check(read(p[0], &c, 1) == 0, "read() of a writerless empty pipe did not return 0");
	pc_pass();
}
