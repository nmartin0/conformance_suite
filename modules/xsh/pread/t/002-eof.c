/*
 * pread() across, at and beyond the end of file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pread.002
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
	char buf[16];
	int fd;

	pc_begin("XSH.pread.002");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_test();
	memset(buf, 0, sizeof buf);
	pc_check(pread(fd, buf, 8, 6) == 4, "pread() across the end did not return 4");
	pc_check(memcmp(buf, "6789", 4) == 0, "pread() across the end returned the wrong bytes");
	pc_check(pread(fd, buf, 4, 10) == 0, "pread() at the end of file did not return 0");
	pc_check(pread(fd, buf, 4, 50) == 0, "pread() beyond the end of file did not return 0");
	pc_pass();
}
