/*
 * SEEK_SET sets the offset.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lseek.001
 * MUTANT    lseek.noop
 * MUTANT    lseek.set_off_by_one
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

	pc_begin("XSH.lseek.001");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_test();
	r = lseek(fd, 6, SEEK_SET);
	pc_check(r == 6, "lseek(fd, 6, SEEK_SET) returned %ld", (long)r);
	pc_check(read(fd, &c, 1) == 1 && c == '6', "the next read did not start at 6");
	pc_pass();
}
