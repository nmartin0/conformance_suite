/*
 * write() past the end grows the file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.002
 * MUTANT    write.short_by_one
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

	pc_begin("XSH.write.002");
	pc_make_file("f", "0123");
	fd = open("f", O_RDWR);
	pc_check(fd >= 0, "cannot open f");
	pc_check(lseek(fd, 2, SEEK_SET) == 2, "lseek failed");
	pc_require_space();
	pc_test();
	pc_check(write(fd, "WXYZ", 4) == 4, "write failed");
	pc_check(pc_file_size("f") == 6, "size %ld, expected 6", pc_file_size("f"));
	pc_check(pc_file_equals("f", "01WXYZ"), "wrong contents");
	pc_pass();
}
