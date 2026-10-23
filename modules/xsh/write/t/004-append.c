/*
 * O_APPEND writes go to the end.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.004
 * MUTANT    write.append_ignored
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

	pc_begin("XSH.write.004");
	pc_make_file("f", "0123");
	fd = open("f", O_RDWR | O_APPEND);
	pc_check(fd >= 0, "cannot open f");
	pc_require_space();
	pc_check(lseek(fd, 1, SEEK_SET) == 1, "lseek failed");
	pc_test();
	pc_check(write(fd, "AB", 2) == 2, "write failed");
	pc_check(lseek(fd, 0, SEEK_CUR) == 6, "the offset is %ld, expected the end 6",
	    (long)lseek(fd, 0, SEEK_CUR));
	pc_check(pc_file_equals("f", "0123AB"), "the data was not appended");
	pc_pass();
}
