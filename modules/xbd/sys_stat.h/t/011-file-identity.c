/*
 * A new file has st_nlink 1; a second name made with link() gives st_nlink
 * 2 and the same st_dev and st_ino, and removing it takes st_nlink back to
 * 1; two different files have different st_dev and st_ino pairs.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_stat.h.011
 */
#include "pc.h"
#include <errno.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void)
{
	struct stat a, b, c;

	pc_begin("XBD.sys_stat.h.011");
	pc_make_file("a", "x");
	pc_make_file("b", "y");
	pc_test();
	pc_check(stat("a", &a) == 0 && stat("b", &b) == 0, "stat failed (%s)", pc_errname(errno));
	pc_check(a.st_nlink == 1 && b.st_nlink == 1, "st_nlink of a file with one name is %ju and %ju", (uintmax_t)a.st_nlink, (uintmax_t)b.st_nlink);
	pc_check(a.st_dev != b.st_dev || a.st_ino != b.st_ino, "two different files have the same st_dev and st_ino");
	if (link("a", "c") == -1)
		pc_unresolved("link failed (%s)", pc_errname(errno));
	pc_check(stat("a", &a) == 0 && stat("c", &c) == 0, "stat failed (%s)", pc_errname(errno));
	pc_check(a.st_dev == c.st_dev && a.st_ino == c.st_ino, "two names of one file have different st_dev or st_ino");
	pc_check(a.st_nlink == 2 && c.st_nlink == 2, "st_nlink of a file with two names is %ju and %ju", (uintmax_t)a.st_nlink, (uintmax_t)c.st_nlink);
	pc_check(unlink("c") == 0, "unlink failed (%s)", pc_errname(errno));
	pc_check(stat("a", &a) == 0 && a.st_nlink == 1, "st_nlink after removing the second name is not 1");
	pc_check(stat("b", &b) == 0 && (a.st_dev != b.st_dev || a.st_ino != b.st_ino), "two different files have the same st_dev and st_ino");
	pc_pass();
}
