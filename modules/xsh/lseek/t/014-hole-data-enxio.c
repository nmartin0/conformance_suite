/*
 * SEEK_HOLE and SEEK_DATA at or beyond the size give ENXIO.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lseek.014
 * MUTANT    lseek.hole_beyond_ok
 * MUTANT    lseek.enxio_as_einval
 * MUTANT    lseek.error_moves_offset
 * VARIANT   lseek.whence_values
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;

	pc_begin("XSH.lseek.014");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_check(lseek(fd, 2, SEEK_SET) == 2, "cannot set the offset to 2");
#if !defined(SEEK_HOLE) || !defined(SEEK_DATA)
	pc_unresolved("<unistd.h> does not define SEEK_HOLE and SEEK_DATA");
#else
	pc_test();
	pc_expect_err(lseek(fd, 10, SEEK_HOLE), ENXIO, "lseek(fd, <size>, SEEK_HOLE)");
	pc_expect_err(lseek(fd, 20, SEEK_DATA), ENXIO, "lseek(fd, <beyond size>, SEEK_DATA)");
	pc_check(lseek(fd, 0, SEEK_CUR) == 2, "the offset changed");
	pc_pass();
#endif
}
