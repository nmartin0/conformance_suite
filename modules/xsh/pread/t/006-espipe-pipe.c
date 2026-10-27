/*
 * pread() on a pipe fails with ESPIPE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pread.006
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
	char buf[4];
	int p[2];

	pc_begin("XSH.pread.006");
	pc_check(pipe(p) == 0, "pipe failed");
	pc_check(write(p[1], "abc", 3) == 3, "write to the pipe failed");
	pc_test();
	pc_expect_err(pread(p[0], buf, 3, 0), ESPIPE, "pread() on a pipe");
	pc_pass();
}
