/*
 * With O_CLOFORK, FD_CLOFORK is set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.012
 * MUTANT    open.clofork_ignored
 * VARIANT   open.accmode_bits
 *
 * Missing macros are header defects (XBD.fcntl.h.002 and .005), so here
 * they are UNRESOLVED.
 */
#include "pc.h"
#include <fcntl.h>

int main(void)
{
	pc_begin("XSH.open.012");
#if !defined(O_CLOFORK) || !defined(FD_CLOFORK)
	pc_fail("<fcntl.h> lacks O_CLOFORK or FD_CLOFORK "
	    "(see XBD.fcntl.h.002, .005)");
#else
	{
		int fd, fl;
		pc_make_file("f", "x");
		pc_test();
		fd = open("f", O_RDONLY | O_CLOFORK);
		pc_expect_ok(fd, "open(\"f\", O_RDONLY|O_CLOFORK)");
		fl = fcntl(fd, F_GETFD);
		pc_expect_ok(fl, "fcntl(F_GETFD)");
		pc_check((fl & FD_CLOFORK) != 0, "FD_CLOFORK is clear");
		pc_pass();
	}
#endif
}
