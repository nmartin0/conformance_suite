/*
 * SEEK_DATA inside data returns the offset itself.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lseek.013
 * MUTANT    lseek.data_enxio
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;
	off_t r;

	pc_begin("XSH.lseek.013");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
#if !defined(SEEK_HOLE) || !defined(SEEK_DATA)
	pc_unresolved("<unistd.h> does not define SEEK_HOLE and SEEK_DATA");
#else
	pc_test();
	r = lseek(fd, 4, SEEK_DATA);
	pc_check(r == 4, "lseek(fd, 4, SEEK_DATA) returned %ld, expected 4", (long)r);
	pc_pass();
#endif
}
