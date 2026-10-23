/*
 * An offset that overflows off_t gives EOVERFLOW.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lseek.011
 * MUTANT    lseek.eoverflow_as_einval
 * MUTANT    lseek.error_moves_offset
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>

int main(void)
{
	off_t max;
	int fd;

	pc_begin("XSH.lseek.011");
	pc_make_file("f", "0123456789");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_check(lseek(fd, 1, SEEK_SET) == 1, "cannot set the offset to 1");
	/* The largest off_t, without signed overflow: all value bits set. */
	max = 0;
	{
		size_t i;
		for (i = 0; i + 1 < sizeof(off_t) * CHAR_BIT; i++)
			max = max * 2 + 1;
	}
	pc_test();
	pc_expect_err(lseek(fd, max, SEEK_CUR), EOVERFLOW,
	    "lseek(fd, <largest off_t>, SEEK_CUR) from offset 1");
	pc_check(lseek(fd, 0, SEEK_CUR) == 1, "the offset changed");
	pc_pass();
}
