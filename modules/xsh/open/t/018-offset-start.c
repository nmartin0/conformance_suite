/*
 * A newly opened descriptor's offset is at the beginning of the file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.018
 * MUTANT    open.offset_at_end
 */
#include "pc.h"
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;
	off_t off;

	pc_begin("XSH.open.018");
	pc_make_file("f", "hello");
	pc_test();
	fd = open("f", O_RDWR);
	pc_expect_ok(fd, "open(\"f\", O_RDWR)");
	off = lseek(fd, 0, SEEK_CUR);
	pc_check(off == 0, "offset is %ld, expected 0", (long)off);
	pc_pass();
}
