/*
 * read() of 0 bytes returns 0 and does nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.read.003
 * MUTANT    read.zero_moves
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

int main(void)
{
	struct stat a, b;
	char buf[1];
	int fd;
	ssize_t r;

	pc_begin("XSH.read.003");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_check(lseek(fd, 3, SEEK_SET) == 3, "lseek failed");
	pc_check(fstat(fd, &a) == 0, "fstat failed");
	pc_test();
	r = read(fd, buf, 0);
	pc_check(r == 0, "read(0) returned %ld", (long)r);
	pc_check(lseek(fd, 0, SEEK_CUR) == 3, "the offset moved");
	pc_check(fstat(fd, &b) == 0, "fstat failed");
	pc_check(a.st_atim.tv_sec == b.st_atim.tv_sec && a.st_atim.tv_nsec == b.st_atim.tv_nsec,
	    "read(0) updated the access time");
	pc_pass();
}
