/*
 * pathconf(_PC_TIMESTAMP_RESOLUTION) is from 1 ns to 1 s.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fpathconf.006
 * MUTANT    fpathconf.resolution_zero
 */
#include "pc.h"
#include <errno.h>
#include <unistd.h>

int main(void)
{
	pc_begin("XSH.fpathconf.006");
#ifndef _PC_TIMESTAMP_RESOLUTION
	pc_unresolved("<unistd.h> does not define _PC_TIMESTAMP_RESOLUTION");
#else
	{
		static const char *const paths[] = { "d", "f" };
		long r;
		int i;

		pc_make_dir("d");
		pc_make_file("f", "data");
		pc_test();
		for (i = 0; i < 2; i++) {
			errno = 0;
			r = pathconf(paths[i], _PC_TIMESTAMP_RESOLUTION);
			pc_check(r >= 1 && r <= 1000000000L, "pathconf(\"%s\", _PC_TIMESTAMP_RESOLUTION) returned %ld "
				 "(errno %s); expected a resolution from 1 to 1000000000 ns", paths[i], r, pc_errname(errno));
		}
	}
	pc_pass();
#endif
}
