/*
 * Seeking beyond EOF works and does not grow the file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lseek.004
 * MUTANT    lseek.extends_with_junk
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;
	off_t r;

	pc_begin("XSH.lseek.004");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDWR);
	pc_check(fd >= 0, "cannot open f");
	pc_test();
	r = lseek(fd, 100, SEEK_SET);
	pc_check(r == 100, "lseek(fd, 100, SEEK_SET) returned %ld", (long)r);
	pc_check(pc_file_size("f") == 10, "the size changed to %ld", pc_file_size("f"));
	pc_pass();
}
