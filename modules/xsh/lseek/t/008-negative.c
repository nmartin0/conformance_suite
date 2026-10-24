/*
 * A negative resulting offset gives EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lseek.008
 * MUTANT    lseek.negative_ok
 * MUTANT    lseek.error_moves_offset
 * MUTANT    lseek.negative_cur_ok
 * VARIANT   lseek.whence_values
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd, dfd;

	pc_begin("XSH.lseek.008");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_check(lseek(fd, 5, SEEK_SET) == 5, "cannot set the offset to 5");
	dfd = open(".", O_RDONLY);
	pc_check(dfd >= 0, "cannot open the directory");
	pc_test();
	pc_expect_err(lseek(fd, -1, SEEK_SET), EINVAL, "lseek(fd, -1, SEEK_SET)");
	pc_check(lseek(fd, 0, SEEK_CUR) == 5, "the offset changed after SEEK_SET -1");
	pc_expect_err(lseek(fd, -6, SEEK_CUR), EINVAL, "lseek(fd, -6, SEEK_CUR) from 5");
	pc_check(lseek(fd, 0, SEEK_CUR) == 5, "the offset changed after SEEK_CUR -6");
	pc_expect_err(lseek(dfd, -1, SEEK_SET), EINVAL, "lseek(<directory>, -1, SEEK_SET)");
	pc_pass();
}
