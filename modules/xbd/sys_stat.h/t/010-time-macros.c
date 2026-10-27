/*
 * st_atime, st_mtime and st_ctime are macros that stand for the tv_sec
 * members of st_atim, st_mtim and st_ctim of the structure they are used
 * with.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_stat.h.010
 */
#include "pc.h"
#include <string.h>
#include <sys/stat.h>

int main(void)
{
	struct stat st;

	pc_begin("XBD.sys_stat.h.010");
#if !defined(st_atime) || !defined(st_mtime) || !defined(st_ctime)
	pc_fail("st_atime, st_mtime or st_ctime is not defined as a macro");
#else
	pc_test();
	memset(&st, 0, sizeof st);
	st.st_atim.tv_sec = 1000001;
	st.st_mtim.tv_sec = 2000002;
	st.st_ctim.tv_sec = 3000003;
	pc_check(st.st_atime == st.st_atim.tv_sec && st.st_atime == (time_t)1000001, "st_atime is not the seconds of st_atim");
	pc_check(st.st_mtime == st.st_mtim.tv_sec && st.st_mtime == (time_t)2000002, "st_mtime is not the seconds of st_mtim");
	pc_check(st.st_ctime == st.st_ctim.tv_sec && st.st_ctime == (time_t)3000003, "st_ctime is not the seconds of st_ctim");
	/* The macro stands for the member, whichever structure it is used with. */
	{
		struct stat other;

		memset(&other, 0, sizeof other);
		other.st_mtim.tv_sec = 77;
		pc_check(other.st_mtime == 77 && other.st_atime == 0 && other.st_ctime == 0, "the macros do not follow the structure they are used with");
	}
	pc_pass();
#endif
}
