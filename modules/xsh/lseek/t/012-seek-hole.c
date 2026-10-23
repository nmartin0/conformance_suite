/*
 * SEEK_HOLE finds the virtual hole at the end of a dense file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lseek.012
 * MUTANT    lseek.hole_at_offset
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;
	off_t r;

	pc_begin("XSH.lseek.012");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
#if !defined(SEEK_HOLE) || !defined(SEEK_DATA)
	pc_unresolved("<unistd.h> does not define SEEK_HOLE and SEEK_DATA");
#else
	pc_test();
	r = lseek(fd, 3, SEEK_HOLE);
	pc_check(r == 10, "lseek(fd, 3, SEEK_HOLE) returned %ld, expected the size 10", (long)r);
	pc_check(lseek(fd, 0, SEEK_CUR) == 10, "the offset was not set to the hole");
	pc_pass();
#endif
}
