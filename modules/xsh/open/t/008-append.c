/*
 * O_APPEND: each write lands at the end of the file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.008
 * MUTANT    open.append_ignored
 */
#include "pc.h"
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;

	pc_begin("XSH.open.008");
	pc_make_file("f", "abc");
	pc_test();
	fd = open("f", O_WRONLY | O_APPEND);
	pc_expect_ok(fd, "open(\"f\", O_WRONLY|O_APPEND)");
	pc_check(lseek(fd, 0, SEEK_SET) == 0, "lseek to 0");
	pc_check(write(fd, "X", 1) == 1, "first write");
	pc_check(lseek(fd, 1, SEEK_SET) == 1, "lseek to 1");
	pc_check(write(fd, "Y", 1) == 1, "second write");
	pc_check(pc_file_equals("f", "abcXY"),
	    "contents are not \"abcXY\" (writes did not go to the end)");
	pc_pass();
}
