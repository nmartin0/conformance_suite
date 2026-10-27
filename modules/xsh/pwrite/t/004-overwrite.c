/*
 * pwrite() inside the file replaces only the bytes written.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pwrite.004
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

	pc_begin("XSH.pwrite.004");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDWR);
	pc_check(fd >= 0, "cannot open f");
	pc_test();
	pc_check(pwrite(fd, "ab", 2, 4) == 2, "pwrite() did not return 2");
	pc_check(pc_file_equals("f", "0123ab6789"), "the contents are wrong");
	pc_pass();
}
