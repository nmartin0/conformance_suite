/*
 * pread() of 0 bytes returns 0 with no other result.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pread.003
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
	int fd;

	pc_begin("XSH.pread.003");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_check(lseek(fd, 4, SEEK_SET) == 4, "lseek failed");
	pc_test();
	pc_check(pread(fd, buf, 0, 2) == 0, "pread() of 0 bytes did not return 0");
	pc_check(lseek(fd, 0, SEEK_CUR) == 4, "the file offset changed");
	pc_pass();
}
