/*
 * O_APPEND does not affect pread().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pread.009
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

	pc_begin("XSH.pread.009");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDWR | O_APPEND);
	pc_check(fd >= 0, "cannot open f");
	pc_check(lseek(fd, 1, SEEK_SET) == 1, "lseek failed");
	pc_test();
	memset(buf, 0, sizeof buf);
	pc_check(pread(fd, buf, 3, 2) == 3, "pread() did not return 3");
	pc_check(memcmp(buf, "234", 3) == 0, "pread() returned the wrong bytes");
	pc_check(lseek(fd, 0, SEEK_CUR) == 1, "the file offset changed");
	pc_pass();
}
