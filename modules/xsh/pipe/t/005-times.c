/*
 * pipe() sets the pipe's timestamps.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pipe.005
 * MUTANT    pipe.old_times
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

int main(void)
{
	struct timespec t0, t1;
	struct stat st;
	int p[2];

	pc_begin("XSH.pipe.005");
	pc_check(clock_gettime(CLOCK_REALTIME, &t0) == 0, "clock_gettime failed");
	pc_test();
	pc_expect_ok(pipe(p), "pipe()");
	pc_check(fstat(p[0], &st) == 0, "fstat failed");
	pc_check(clock_gettime(CLOCK_REALTIME, &t1) == 0, "clock_gettime failed");
	pc_check(st.st_atim.tv_sec <= t1.tv_sec + 1 && st.st_mtim.tv_sec <= t1.tv_sec + 1 &&
	    st.st_ctim.tv_sec <= t1.tv_sec + 1, "a timestamp is in the future");
	pc_check(st.st_atim.tv_sec >= t0.tv_sec - 1, "atime %ld is before the call (%ld)",
	    (long)st.st_atim.tv_sec, (long)t0.tv_sec);
	pc_check(st.st_mtim.tv_sec >= t0.tv_sec - 1, "mtime %ld is before the call (%ld)",
	    (long)st.st_mtim.tv_sec, (long)t0.tv_sec);
	pc_check(st.st_ctim.tv_sec >= t0.tv_sec - 1, "ctime %ld is before the call (%ld)",
	    (long)st.st_ctim.tv_sec, (long)t0.tv_sec);
	pc_pass();
}
