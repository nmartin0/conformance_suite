/*
 * A descriptor that catopen() creates is close-on-exec.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.catopen.005
 */
#include "cg.h"

int main(void)
{
	static unsigned char before[CG_FDS], after[CG_FDS];
	nl_catd cd;
	int fd, flags;

	pc_begin("XSH.catopen.005");
	cg_make("c005.cat", "c005.msg", CG_SRC);
	pc_test();
	cg_fdmap(before);
	cd = catopen("./c005.cat", 0);
	pc_check(cd != CG_BAD, "catopen failed: %s", pc_errname(errno));
	cg_fdmap(after);
	for (fd = 0; fd < CG_FDS; fd++) {
		if (after[fd] && !before[fd]) {
			flags = fcntl(fd, F_GETFD);
			pc_check(flags != -1 && (flags & FD_CLOEXEC) != 0,
			    "descriptor %d opened by catopen() lacks FD_CLOEXEC", fd);
		}
	}
	(void)catclose(cd);
	pc_pass();
}
