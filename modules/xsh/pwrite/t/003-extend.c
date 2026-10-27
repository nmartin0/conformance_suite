/*
 * pwrite() beyond the end extends the file; the gap reads zeros.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pwrite.003
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
	char buf[12];
	int fd, i;

	pc_begin("XSH.pwrite.003");
	pc_make_file("f", "abc");
	fd = open("f", O_RDWR);
	pc_check(fd >= 0, "cannot open f");
	pc_test();
	pc_check(pwrite(fd, "XY", 2, 10) == 2, "pwrite() did not return 2");
	pc_check(pc_file_size("f") == 12, "the length is %ld, not 12", pc_file_size("f"));
	memset(buf, 'q', sizeof buf);
	pc_check(pread(fd, buf, sizeof buf, 0) == (ssize_t)sizeof buf, "pread() did not return 12");
	pc_check(memcmp(buf, "abc", 3) == 0 && memcmp(buf + 10, "XY", 2) == 0, "the data is wrong");
	for (i = 3; i < 10; i++)
		pc_check(buf[i] == 0, "gap byte %d is %d, not 0", i, buf[i]);
	pc_pass();
}
