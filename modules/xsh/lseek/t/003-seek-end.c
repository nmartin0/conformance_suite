/*
 * SEEK_END measures from the end of file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lseek.003
 * MUTANT    lseek.end_ignores_offset
 * VARIANT   lseek.whence_values
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;
	off_t r;
	char c;

	pc_begin("XSH.lseek.003");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_test();
	r = lseek(fd, -3, SEEK_END);
	pc_check(r == 7, "lseek(fd, -3, SEEK_END) on 10 bytes returned %ld", (long)r);
	pc_check(read(fd, &c, 1) == 1 && c == '7', "the next read did not start at 7");
	r = lseek(fd, 0, SEEK_END);
	pc_check(r == 10, "lseek(fd, 0, SEEK_END) returned %ld", (long)r);
	pc_pass();
}
