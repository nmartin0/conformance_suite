/*
 * For st_mode of a real regular file, directory, FIFO and symbolic link
 * (lstat), the matching S_IS macro is true and the others are false;
 * stat() of the symbolic link gives the regular file it names.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_stat.h.006
 */
#include "pc.h"
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

static void expect(const char *what, mode_t m, int reg, int dir, int fifo, int lnk)
{
	pc_check((!!S_ISREG(m)) == reg, "S_ISREG is %d for %s (mode %#o)", !!S_ISREG(m), what, (unsigned)m);
	pc_check((!!S_ISDIR(m)) == dir, "S_ISDIR is %d for %s (mode %#o)", !!S_ISDIR(m), what, (unsigned)m);
	pc_check((!!S_ISFIFO(m)) == fifo, "S_ISFIFO is %d for %s (mode %#o)", !!S_ISFIFO(m), what, (unsigned)m);
	pc_check((!!S_ISLNK(m)) == lnk, "S_ISLNK is %d for %s (mode %#o)", !!S_ISLNK(m), what, (unsigned)m);
	pc_check(!S_ISBLK(m) && !S_ISCHR(m) && !S_ISSOCK(m), "a block, character or socket test is true for %s (mode %#o)", what, (unsigned)m);
}

int main(void)
{
	struct stat st;

	pc_begin("XBD.sys_stat.h.006");
	pc_make_file("f", "x");
	pc_make_dir("d");
	if (mkfifo("p", 0600) == -1)
		pc_unresolved("mkfifo failed (%s)", pc_errname(errno));
	pc_make_symlink("f", "l");
	pc_test();
	pc_check(stat("f", &st) == 0, "stat(f) failed (%s)", pc_errname(errno));
	expect("a regular file", st.st_mode, 1, 0, 0, 0);
	pc_check(stat("d", &st) == 0, "stat(d) failed (%s)", pc_errname(errno));
	expect("a directory", st.st_mode, 0, 1, 0, 0);
	pc_check(stat("p", &st) == 0, "stat(p) failed (%s)", pc_errname(errno));
	expect("a FIFO", st.st_mode, 0, 0, 1, 0);
	pc_check(lstat("l", &st) == 0, "lstat(l) failed (%s)", pc_errname(errno));
	expect("a symbolic link", st.st_mode, 0, 0, 0, 1);
	pc_check(stat("l", &st) == 0, "stat(l) failed (%s)", pc_errname(errno));
	expect("the file a symbolic link names", st.st_mode, 1, 0, 0, 0);
	pc_pass();
}
