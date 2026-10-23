/*
 * lseek() on a closed descriptor gives EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lseek.010
 * MUTANT    lseek.ebadf_as_einval
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int bad;

	pc_begin("XSH.lseek.010");
	bad = pc_lowest_free_fd();
	pc_test();
	pc_expect_err(lseek(bad, 0, SEEK_SET), EBADF, "lseek(<not open>)");
	pc_pass();
}
