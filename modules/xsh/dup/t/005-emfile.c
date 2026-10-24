/*
 * dup() with every descriptor in use gives EMFILE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.dup.005
 * MUTANT    dup.emfile_as_enfile
 * VARIANT   dup.hidden_fds
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/resource.h>

int main(void)
{
	struct rlimit rl;
	int i, src, lim, top;

	pc_begin("XSH.dup.005");
	pc_make_file("f", "x");
	src = open("f", O_RDONLY);
	pc_check(src >= 0, "cannot open f");
	lim = pc_lowest_free_fd() + 2;
	if (lim < 20)
		lim = 20;	/* not below {_POSIX_OPEN_MAX}: getrlimit(), APPLICATION USAGE */
	/*
	 * Never lower the limit below a descriptor that is open (getrlimit(),
	 * APPLICATION USAGE). The implementation may hold descriptors of its
	 * own at any number, so look up to the current limit.
	 */
	pc_check(getrlimit(RLIMIT_NOFILE, &rl) == 0, "getrlimit failed");
	top = rl.rlim_cur == RLIM_INFINITY || rl.rlim_cur > 65536 ? 65536 :
	    (int)rl.rlim_cur;
	for (i = 0; i < top; i++)
		if (fcntl(i, F_GETFD) != -1 && i + 1 > lim)
			lim = i + 1;
	if (lim > 4096)
		pc_unresolved("a descriptor near %d is open; too many to fill", lim);
	for (i = 0; i < lim; i++)
		if (fcntl(i, F_GETFD) == -1)
			pc_check(dup2(src, i) == i, "dup2 to %d failed", i);
	rl.rlim_cur = (rlim_t)lim;
	pc_check(setrlimit(RLIMIT_NOFILE, &rl) == 0, "setrlimit failed");
	if (getrlimit(RLIMIT_NOFILE, &rl) != 0 || rl.rlim_cur != (rlim_t)lim)
		pc_unresolved("RLIMIT_NOFILE did not take the value %d", lim);
	for (i = 0; i < lim; i++)
		pc_check(fcntl(i, F_GETFD) != -1, "descriptor %d is not in use", i);
	pc_test();
	pc_expect_err(dup(src), EMFILE, "dup() with every descriptor in use");
	pc_pass();
}
