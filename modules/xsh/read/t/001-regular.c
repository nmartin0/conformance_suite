/*
 * read() reads at the offset and advances it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.read.001
 * MUTANT    read.short_by_one
 * MUTANT    read.offset_not_advanced
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	char buf[8];
	int fd;
	ssize_t r;

	pc_begin("XSH.read.001");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_check(lseek(fd, 2, SEEK_SET) == 2, "lseek failed");
	pc_test();
	r = read(fd, buf, 5);
	pc_check(r == 5, "read(5) returned %ld", (long)r);
	pc_check(memcmp(buf, "23456", 5) == 0, "read the wrong bytes");
	pc_check(lseek(fd, 0, SEEK_CUR) == 7, "the offset is %ld, expected 7",
	    (long)lseek(fd, 0, SEEK_CUR));
	pc_pass();
}
