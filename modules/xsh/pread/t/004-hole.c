/*
 * pread() of a never-written range before the end reads zeros.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pread.004
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
	unsigned char buf[16];
	int fd, i;

	pc_begin("XSH.pread.004");
	pc_make_file("f", "");
	fd = open("f", O_RDWR);
	pc_check(fd >= 0, "cannot open f");
	pc_check(ftruncate(fd, 100) == 0, "ftruncate failed");
	memset(buf, 0xff, sizeof buf);
	pc_test();
	pc_check(pread(fd, buf, sizeof buf, 40) == (ssize_t)sizeof buf, "pread() did not return 16");
	for (i = 0; i < (int)sizeof buf; i++)
		pc_check(buf[i] == 0, "byte %d of the never-written range is %d", i, buf[i]);
	pc_pass();
}
