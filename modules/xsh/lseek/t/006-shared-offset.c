/*
 * The offset belongs to the open file description.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lseek.006
 * MUTANT    lseek.noop
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd, d;
	char c;

	pc_begin("XSH.lseek.006");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	d = dup(fd);
	pc_check(d >= 0, "dup failed");
	pc_test();
	pc_check(lseek(fd, 8, SEEK_SET) == 8, "lseek to 8 failed");
	pc_check(read(d, &c, 1) == 1 && c == '8', "the duplicate does not read from 8");
	pc_pass();
}
