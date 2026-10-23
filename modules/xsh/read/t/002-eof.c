/*
 * read() stops at the end of file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.read.002
 * MUTANT    read.past_eof
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	char buf[16];
	int fd;
	ssize_t r;

	pc_begin("XSH.read.002");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_check(lseek(fd, 6, SEEK_SET) == 6, "lseek failed");
	pc_test();
	r = read(fd, buf, sizeof buf);
	pc_check(r == 4, "read(16) with 4 bytes left returned %ld", (long)r);
	pc_check(memcmp(buf, "6789", 4) == 0, "read the wrong bytes");
	r = read(fd, buf, sizeof buf);
	pc_check(r == 0, "read() at the end of file returned %ld", (long)r);
	pc_pass();
}
