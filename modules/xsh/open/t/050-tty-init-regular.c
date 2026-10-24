/*
 * O_TTY_INIT on a regular file is ignored.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.050
 * MUTANT    open.tty_init_enotty
 * VARIANT   open.accmode_bits
 * VARIANT   open.tty_init_bit
 */
#include "pc.h"
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
#ifdef O_TTY_INIT
	int fd;
	char c;
#endif

	pc_begin("XSH.open.050");
	pc_make_file("f", "x");
#ifndef O_TTY_INIT
	pc_unresolved("<fcntl.h> does not define O_TTY_INIT");
#else
	if (O_TTY_INIT == 0)
		pc_notinuse("O_TTY_INIT is 0, which is allowed: there is no flag to ignore");
	pc_test();
	fd = open("f", O_RDONLY | O_TTY_INIT);
	pc_expect_ok(fd, "open(\"f\", O_RDONLY|O_TTY_INIT)");
	pc_check(read(fd, &c, 1) == 1 && c == 'x', "read() failed");
	pc_pass();
#endif
}
