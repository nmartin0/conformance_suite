/*
 * The file offset after creat() is 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.creat.005
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

	pc_begin("XSH.creat.005");
	pc_make_file("f", "old contents");
	pc_test();
	fd = creat("f", 0600);
	pc_expect_ok(fd, "creat(\"f\")");
	pc_check(lseek(fd, 0, SEEK_CUR) == 0, "the file offset is not 0");
	pc_check(write(fd, "ab", 2) == 2, "write failed");
	pc_check(pc_file_equals("f", "ab"), "the write did not land at byte 0 of the emptied file");
	pc_pass();
}
