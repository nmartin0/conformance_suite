/*
 * sysconf() reports the POSIX.1-2024 version values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sysconf.005
 * MUTANT    sysconf.version_2008
 * MUTANT    sysconf.xopen_700
 */
#include "pc.h"
#include <errno.h>
#include <unistd.h>

int main(void)
{
	long v, v2, x = 0;
	int xsi;

	pc_begin("XSH.sysconf.005");
#ifdef _POSIX_SUBPROFILE
	pc_notinuse("a subprofile implementation (_POSIX_SUBPROFILE)");
#endif
	pc_test();
	v = sysconf(_SC_VERSION);
	v2 = sysconf(_SC_2_VERSION);
	xsi = sysconf(_SC_XOPEN_UNIX) > 0;
	if (xsi)
		x = sysconf(_SC_XOPEN_VERSION);
	pc_check(v == 202405L, "sysconf(_SC_VERSION) returned %ld, expected 202405", v);
	pc_check(v2 == 202405L, "sysconf(_SC_2_VERSION) returned %ld, expected 202405", v2);
	pc_check(!xsi || x == 800, "sysconf(_SC_XOPEN_VERSION) returned %ld on an XSI system, expected 800", x);
	pc_pass();
}
