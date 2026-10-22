/*
 * Without O_CLOFORK, FD_CLOFORK is clear.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.011
 * MUTANT    open.clofork_forced
 *
 * A missing FD_CLOFORK is a header defect (XBD.fcntl.h.002), not an open()
 * one, so here it is UNRESOLVED.
 */
#include "pc.h"
#include <fcntl.h>

int main(void)
{
	pc_begin("XSH.open.011");
#ifndef FD_CLOFORK
	pc_unresolved("<fcntl.h> lacks FD_CLOFORK (see XBD.fcntl.h.002)");
#else
	{
		int fd, fl;
		pc_make_file("f", "x");
		pc_test();
		fd = open("f", O_RDONLY);
		pc_expect_ok(fd, "open(\"f\", O_RDONLY)");
		fl = fcntl(fd, F_GETFD);
		pc_expect_ok(fl, "fcntl(F_GETFD)");
		pc_check((fl & FD_CLOFORK) == 0, "FD_CLOFORK is set");
		pc_pass();
	}
#endif
}
