/*
 * read() beyond the end of file returns 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.read.004
 * MUTANT    read.past_eof
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	char buf[4];
	int fd;

	pc_begin("XSH.read.004");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_check(lseek(fd, 50, SEEK_SET) == 50, "lseek failed");
	pc_test();
	pc_check(read(fd, buf, sizeof buf) == 0, "read() beyond the end of file did not return 0");
	pc_pass();
}
