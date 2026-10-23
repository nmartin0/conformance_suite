/*
 * pwrite() ignores O_APPEND.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.022
 * MUTANT    write.pwrite_appends
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

	pc_begin("XSH.write.022");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDWR | O_APPEND);
	pc_check(fd >= 0, "cannot open f");
	pc_test();
	pc_check(pwrite(fd, "XY", 2, 3) == 2, "pwrite failed");
	pc_check(pc_file_equals("f", "012XY56789"), "pwrite with O_APPEND did not write at offset 3");
	pc_pass();
}
