/*
 * S_TYPEISTMO() applied to the stat structure of a regular file and of a
 * directory evaluates to zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_stat.h.008
 */
#include "pc.h"
#include <errno.h>
#include <sys/stat.h>

int main(void)
{
	pc_begin("XBD.sys_stat.h.008");
#if !defined(S_TYPEISTMO)
	pc_unresolved("a test macro is missing (the header check reports it): S_TYPEISTMO");
#else
	struct stat st;

	pc_make_file("f", "x");
	pc_make_dir("d");
	pc_test();
	pc_check(stat("f", &st) == 0, "stat(f) failed (%s)", pc_errname(errno));
	pc_check(S_TYPEISTMO(&st) == 0, "S_TYPEISTMO is nonzero for a regular file");
	pc_check(stat("d", &st) == 0, "stat(d) failed (%s)", pc_errname(errno));
	pc_check(S_TYPEISTMO(&st) == 0, "S_TYPEISTMO is nonzero for a directory");
	pc_pass();
#endif
}
