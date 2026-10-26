/*
 * sysconf() of clock ticks per second and of the initial getgr*_r() and
 * getpw*_r() buffer sizes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sysconf.013
 * MUTANT    sysconf.clk_tck_zero
 * MUTANT    sysconf.getpw_sets_errno
 * VARIANT   sysconf.errno_on_success
 */
#include "pc.h"
#include <errno.h>
#include <unistd.h>

#define SENTINEL 12345

int main(void)
{
	static const struct {
		const char *name;
		int sc;
	} sizes[] = { { "_SC_GETGR_R_SIZE_MAX", _SC_GETGR_R_SIZE_MAX }, { "_SC_GETPW_R_SIZE_MAX", _SC_GETPW_R_SIZE_MAX } };
	long r;
	size_t i;
	int e;

	pc_begin("XSH.sysconf.013");
	pc_test();
	r = sysconf(_SC_CLK_TCK);
	pc_check(r > 0, "sysconf(_SC_CLK_TCK) returned %ld; clock ticks per second must be a positive count", r);
	for (i = 0; i < 2; i++) {
		errno = SENTINEL;
		r = sysconf(sizes[i].sc);
		e = errno;
		if (r == -1)
			pc_check(e == SENTINEL, "sysconf(%s) returned -1 and changed errno to %s", sizes[i].name,
				 pc_errname(e));
		else
			pc_check(r > 0, "sysconf(%s) returned %ld, not an initial buffer size", sizes[i].name, r);
	}
	pc_pass();
}
