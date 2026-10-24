/*
 * An invalid whence gives EINVAL and changes nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lseek.007
 * MUTANT    lseek.bad_whence_ok
 * MUTANT    lseek.error_moves_offset
 * MUTANT    lseek.einval_as_eoverflow
 * VARIANT   lseek.whence_values
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;

	pc_begin("XSH.lseek.007");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_check(lseek(fd, 5, SEEK_SET) == 5, "cannot set the offset to 5");
	pc_test();
	pc_expect_err(lseek(fd, 1, 12345), EINVAL, "lseek(fd, 1, <invalid whence>)");
	pc_check(lseek(fd, 0, SEEK_CUR) == 5, "the offset changed");
	pc_pass();
}
