/*
 * pwrite() writes at the given offset and leaves the file offset alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pwrite.001
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
	int fd;

	pc_begin("XSH.pwrite.001");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDWR);
	pc_check(fd >= 0, "cannot open f");
	pc_check(lseek(fd, 2, SEEK_SET) == 2, "lseek failed");
	pc_test();
	pc_check(pwrite(fd, "XY", 2, 7) == 2, "pwrite() did not return 2");
	pc_check(pc_file_equals("f", "0123456XY9"), "pwrite() wrote in the wrong place");
	pc_check(lseek(fd, 0, SEEK_CUR) == 2, "the file offset changed");
	pc_pass();
}
