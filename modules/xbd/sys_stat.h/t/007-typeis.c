/*
 * S_TYPEISMQ(), S_TYPEISSEM() and S_TYPEISSHM() applied to the stat
 * structure of a regular file and of a directory evaluate to zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_stat.h.007
 */
#include "pc.h"
#include <errno.h>
#include <sys/stat.h>

int main(void)
{
	pc_begin("XBD.sys_stat.h.007");
#if !defined(S_TYPEISMQ) || !defined(S_TYPEISSEM) || !defined(S_TYPEISSHM)
	pc_unresolved("a test macro is missing (the header check reports it): S_TYPEISMQ S_TYPEISSEM S_TYPEISSHM");
#else
	struct stat st;

	pc_make_file("f", "x");
	pc_make_dir("d");
	pc_test();
	pc_check(stat("f", &st) == 0, "stat(f) failed (%s)", pc_errname(errno));
	pc_check(S_TYPEISMQ(&st) == 0, "S_TYPEISMQ is nonzero for a regular file");
	pc_check(S_TYPEISSEM(&st) == 0, "S_TYPEISSEM is nonzero for a regular file");
	pc_check(S_TYPEISSHM(&st) == 0, "S_TYPEISSHM is nonzero for a regular file");
	pc_check(stat("d", &st) == 0, "stat(d) failed (%s)", pc_errname(errno));
	pc_check(S_TYPEISMQ(&st) == 0, "S_TYPEISMQ is nonzero for a directory");
	pc_check(S_TYPEISSEM(&st) == 0, "S_TYPEISSEM is nonzero for a directory");
	pc_check(S_TYPEISSHM(&st) == 0, "S_TYPEISSHM is nonzero for a directory");
	pc_pass();
#endif
}
