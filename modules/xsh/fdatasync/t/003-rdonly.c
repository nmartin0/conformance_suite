/*
 * fdatasync() of a read-only descriptor returns 0, with writes pending through another.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fdatasync.003
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
	int w, r;

	pc_begin("XSH.fdatasync.003");
	pc_require_space();
	w = open("f", O_WRONLY | O_CREAT, 0600);
	pc_check(w >= 0, "cannot create f");
	pc_check(write(w, "abcdef", 6) == 6, "write failed");
	r = open("f", O_RDONLY);
	pc_check(r >= 0, "cannot reopen f");
#ifdef _PC_SYNC_IO
	errno = 0;
	if (fpathconf(r, _PC_SYNC_IO) == -1 && errno == 0)
		pc_notinuse("synchronized I/O is not supported for this file");
#endif
	pc_test();
	pc_check(fdatasync(r) == 0, "fdatasync() failed: %s", pc_errname(errno));
	pc_check(pc_file_equals("f", "abcdef"), "the contents changed");
	pc_pass();
}
