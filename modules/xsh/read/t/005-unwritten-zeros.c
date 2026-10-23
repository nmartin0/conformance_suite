/*
 * Unwritten bytes read as zeros.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.read.005
 * MUTANT    read.zeros_as_junk
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	char buf[10];
	int fd, i;

	pc_begin("XSH.read.005");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDWR);
	pc_check(fd >= 0, "cannot open f");
	pc_check(ftruncate(fd, 20) == 0, "ftruncate failed");
	pc_check(lseek(fd, 10, SEEK_SET) == 10, "lseek failed");
	pc_test();
	pc_check(read(fd, buf, 10) == 10, "read of the unwritten part failed");
	for (i = 0; i < 10; i++)
		pc_check(buf[i] == 0, "byte %d is %d, expected 0", 10 + i, buf[i]);
	pc_pass();
}
