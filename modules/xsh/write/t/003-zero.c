/*
 * write() of 0 bytes does nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.003
 * MUTANT    write.zero_writes
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	int fd;

	pc_begin("XSH.write.003");
	pc_make_file("f", "0123");
	fd = open("f", O_RDWR);
	pc_check(fd >= 0, "cannot open f");
	pc_check(lseek(fd, 1, SEEK_SET) == 1, "lseek failed");
	pc_test();
	pc_check(write(fd, "x", 0) == 0, "write(0) did not return 0");
	pc_check(pc_file_equals("f", "0123"), "the file changed");
	pc_check(lseek(fd, 0, SEEK_CUR) == 1, "the offset moved");
	pc_pass();
}
