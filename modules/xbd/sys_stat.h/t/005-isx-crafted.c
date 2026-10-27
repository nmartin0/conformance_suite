/*
 * For a mode made of one of the seven file type constants and any
 * permission bits (0, 0644, 0777, 07777), exactly the matching one of
 * S_ISBLK, S_ISCHR, S_ISDIR, S_ISFIFO, S_ISREG, S_ISLNK and S_ISSOCK is
 * nonzero and the other six are zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_stat.h.005
 */
#include "pc.h"
#include <sys/stat.h>

#if defined(S_IFBLK) && defined(S_IFCHR) && defined(S_IFIFO) && defined(S_IFREG) && defined(S_IFDIR) && defined(S_IFLNK) && defined(S_IFSOCK)
static const char *const tn[7] = { "block special", "character special", "directory", "FIFO", "regular", "symbolic link", "socket" };

/* Mode made of type t and permission bits perm: exactly the macro `which` is true. */
static void one(int which, mode_t t, unsigned perm)
{
	mode_t m = (mode_t)(t | perm);
	int r[7], k;

	r[0] = S_ISBLK(m);
	r[1] = S_ISCHR(m);
	r[2] = S_ISDIR(m);
	r[3] = S_ISFIFO(m);
	r[4] = S_ISREG(m);
	r[5] = S_ISLNK(m);
	r[6] = S_ISSOCK(m);
	for (k = 0; k < 7; k++) {
		if (k == which)
			pc_check(r[k] != 0, "the test for a %s file is false for mode %#o of that type", tn[k], (unsigned)m);
		else
			pc_check(r[k] == 0, "the test for a %s file is true for mode %#o of a %s file", tn[k], (unsigned)m, tn[which]);
	}
}
#endif

int main(void)
{
	pc_begin("XBD.sys_stat.h.005");
#if !defined(S_IFBLK) || !defined(S_IFCHR) || !defined(S_IFIFO) || !defined(S_IFREG) || !defined(S_IFDIR) || !defined(S_IFLNK) || !defined(S_IFSOCK) || !defined(S_ISBLK) || !defined(S_ISCHR) || !defined(S_ISDIR) || !defined(S_ISFIFO) || !defined(S_ISREG) || !defined(S_ISLNK) || !defined(S_ISSOCK)
	pc_unresolved("a file type constant or test macro is missing (the header check reports it)");
#else
	{
		static const unsigned perms[] = { 0, 0644, 0777, 07777 };
		mode_t types[7];
		int k, p;

		types[0] = S_IFBLK;
		types[1] = S_IFCHR;
		types[2] = S_IFDIR;
		types[3] = S_IFIFO;
		types[4] = S_IFREG;
		types[5] = S_IFLNK;
		types[6] = S_IFSOCK;
		pc_test();
		for (k = 0; k < 7; k++)
			for (p = 0; p < 4; p++)
				one(k, types[k], perms[p]);
		pc_pass();
	}
#endif
}
