/*
 * A gap left by seeking past EOF reads as zeros.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lseek.005
 * MUTANT    lseek.extends_with_junk
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	char buf[16];
	int fd, i;

	pc_begin("XSH.lseek.005");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDWR);
	pc_check(fd >= 0, "cannot open f");
	pc_test();
	pc_check(lseek(fd, 14, SEEK_SET) == 14, "lseek to 14 failed");
	pc_check(write(fd, "E", 1) == 1, "write at 14 failed");
	pc_check(pc_file_size("f") == 15, "size %ld after writing at 14", pc_file_size("f"));
	pc_check(lseek(fd, 10, SEEK_SET) == 10, "lseek to 10 failed");
	pc_check(read(fd, buf, 5) == 5, "reading the gap failed");
	for (i = 0; i < 4; i++)
		pc_check(buf[i] == 0, "gap byte %d is %d, expected 0", 10 + i, buf[i]);
	pc_check(buf[4] == 'E', "the written byte is not at 14");
	pc_pass();
}
