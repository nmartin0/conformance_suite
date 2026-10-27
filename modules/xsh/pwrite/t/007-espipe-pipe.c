/*
 * pwrite() on a pipe fails with ESPIPE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pwrite.007
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
	int p[2];

	pc_begin("XSH.pwrite.007");
	pc_check(pipe(p) == 0, "pipe failed");
	pc_test();
	pc_expect_err(pwrite(p[1], "x", 1, 0), ESPIPE, "pwrite() on a pipe");
	pc_pass();
}
