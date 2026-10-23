/*
 * pread() reads at an offset without moving the file offset.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.read.015
 * MUTANT    read.pread_ignores_offset
 * MUTANT    read.pread_moves_offset
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	char buf[4];
	int fd;
	ssize_t r;

	pc_begin("XSH.read.015");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_check(lseek(fd, 1, SEEK_SET) == 1, "lseek failed");
	pc_test();
	r = pread(fd, buf, 3, 6);
	pc_check(r == 3 && memcmp(buf, "678", 3) == 0, "pread(3 at 6) returned %ld or wrong bytes", (long)r);
	pc_check(lseek(fd, 0, SEEK_CUR) == 1, "the file offset changed");
	pc_pass();
}
