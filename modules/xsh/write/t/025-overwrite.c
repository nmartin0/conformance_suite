/*
 * Later writes overwrite earlier ones.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.025
 * MUTANT    write.offset_not_advanced
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

	pc_begin("XSH.write.025");
	pc_make_file("f", "");
	fd = open("f", O_RDWR);
	pc_check(fd >= 0, "cannot open f");
	pc_require_space();
	pc_test();
	pc_check(write(fd, "aaaa", 4) == 4, "first write failed");
	pc_check(lseek(fd, 1, SEEK_SET) == 1, "lseek failed");
	pc_check(write(fd, "bb", 2) == 2, "second write failed");
	pc_check(write(fd, "c", 1) == 1, "third write failed");
	pc_check(pc_file_equals("f", "abbc"), "the file does not hold the latest data");
	pc_pass();
}
