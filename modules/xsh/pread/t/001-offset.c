/*
 * pread() reads at the given offset and leaves the file offset alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pread.001
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
	char buf[8];
	int fd;

	pc_begin("XSH.pread.001");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_check(lseek(fd, 2, SEEK_SET) == 2, "lseek failed");
	pc_test();
	memset(buf, 0, sizeof buf);
	pc_check(pread(fd, buf, 3, 6) == 3, "pread() did not return 3");
	pc_check(memcmp(buf, "678", 3) == 0, "pread() returned the wrong bytes");
	pc_check(lseek(fd, 0, SEEK_CUR) == 2, "the file offset changed");
	pc_check(read(fd, buf, 2) == 2 && memcmp(buf, "23", 2) == 0, "read() after pread() did not continue from 2");
	pc_pass();
}
