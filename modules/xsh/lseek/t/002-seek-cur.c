/*
 * SEEK_CUR moves relative to the current offset.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lseek.002
 * MUTANT    lseek.cur_as_set
 * MUTANT    lseek.noop
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

	pc_begin("XSH.lseek.002");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_check(lseek(fd, 4, SEEK_SET) == 4, "cannot set the offset to 4");
	pc_test();
	r = lseek(fd, 3, SEEK_CUR);
	pc_check(r == 7, "lseek(fd, 3, SEEK_CUR) from 4 returned %ld", (long)r);
	r = lseek(fd, -5, SEEK_CUR);
	pc_check(r == 2, "lseek(fd, -5, SEEK_CUR) from 7 returned %ld", (long)r);
	pc_check(read(fd, &c, 1) == 1 && c == '2', "the next read did not start at 2");
	pc_pass();
}
