/*
 * pwrite() writes at an offset without moving the file offset.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.021
 * MUTANT    write.pwrite_ignores_offset
 * MUTANT    write.pwrite_moves_offset
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

	pc_begin("XSH.write.021");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDWR);
	pc_check(fd >= 0, "cannot open f");
	pc_check(lseek(fd, 2, SEEK_SET) == 2, "lseek failed");
	pc_test();
	pc_check(pwrite(fd, "XY", 2, 7) == 2, "pwrite failed");
	pc_check(pc_file_equals("f", "0123456XY9"), "pwrite wrote in the wrong place");
	pc_check(lseek(fd, 0, SEEK_CUR) == 2, "the file offset changed");
	pc_pass();
}
