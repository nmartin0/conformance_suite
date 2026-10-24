/*
 * posix_close() with an invalid flag still closes the descriptor.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.close.010
 * MUTANT    close.pc_einval_open
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd, r, e, flag;

	pc_begin("XSH.close.010");
	pc_make_file("f", "x");
#ifndef POSIX_CLOSE_RESTART
	(void)fd; (void)r; (void)e; (void)flag;
	pc_unresolved("<unistd.h> does not define POSIX_CLOSE_RESTART (posix_close() missing)");
#else
	flag = 0x40000000;
	if (flag & POSIX_CLOSE_RESTART)
		flag = 0x20000000;
	pc_check(!(flag & POSIX_CLOSE_RESTART), "cannot pick an invalid flag");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_test();
	r = posix_close(fd, flag);
	e = errno;
	pc_check(r == 0 || (r == -1 && e == EINVAL),
	    "posix_close() with an invalid flag returned %d (%s)", r,
	    r == -1 ? pc_errname(e) : "-");
	if (r == 0 && fcntl(fd, F_GETFD) != -1)
		pc_unresolved("posix_close() accepted the flag and left the descriptor open: the bit may be an implementation's own flag");
	pc_expect_err(fcntl(fd, F_GETFD), EBADF,
	    "fcntl(F_GETFD) after posix_close() with an invalid flag");
	pc_pass();
#endif
}
