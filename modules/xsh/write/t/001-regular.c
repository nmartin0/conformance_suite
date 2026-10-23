/*
 * write() writes at the offset and advances it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.001
 * MUTANT    write.short_by_one
 * MUTANT    write.offset_not_advanced
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
	ssize_t r;

	pc_begin("XSH.write.001");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDWR);
	pc_check(fd >= 0, "cannot open f");
	pc_check(lseek(fd, 3, SEEK_SET) == 3, "lseek failed");
	pc_test();
	r = write(fd, "abcd", 4);
	pc_check(r == 4, "write(4) returned %ld", (long)r);
	pc_check(lseek(fd, 0, SEEK_CUR) == 7, "the offset is %ld, expected 7", (long)lseek(fd, 0, SEEK_CUR));
	pc_check(pc_file_equals("f", "012abcd789"), "the file does not hold the data");
	pc_pass();
}
