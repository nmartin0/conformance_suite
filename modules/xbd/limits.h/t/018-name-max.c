/*
 * A file name of {NAME_MAX} bytes works; one byte more is refused.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.limits.h.018
 */
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "pc.h"

int main(void)
{
	long nm, notrunc;
	static char fname[2048], dname[2048], over[2048];
	struct stat st;
	DIR *d;
	struct dirent *e;
	int fd, sawf = 0, sawd = 0;
	size_t i;

	pc_begin("XBD.limits.h.018");
	nm = pathconf(".", _PC_NAME_MAX);
	notrunc = pathconf(".", _PC_NO_TRUNC);
	if (nm == -1)
		pc_notinuse("{NAME_MAX} has no definite value for this directory");
	if (nm > 1024)
		pc_unresolved("{NAME_MAX} is %ld, larger than this test builds", nm);
	pc_test();
	pc_check(nm >= _POSIX_NAME_MAX, "pathconf(_PC_NAME_MAX) is %ld, below %d", nm, _POSIX_NAME_MAX);
	for (i = 0; i < (size_t)nm; i++) {
		fname[i] = 'f';
		dname[i] = 'd';
		over[i] = 'o';
	}
	over[nm] = 'o';

	fd = open(fname, O_WRONLY | O_CREAT | O_EXCL, 0600);
	pc_check(fd != -1, "creating a file whose name has %ld bytes failed with %s", nm, pc_errname(errno));
	(void)close(fd);
	pc_check(mkdir(dname, 0700) == 0, "mkdir() of a name with %ld bytes failed with %s", nm, pc_errname(errno));
	pc_check(stat(fname, &st) == 0 && S_ISREG(st.st_mode), "stat() of the %ld-byte file name failed", nm);
	pc_check(stat(dname, &st) == 0 && S_ISDIR(st.st_mode), "stat() of the %ld-byte directory name failed", nm);

	d = opendir(".");
	pc_check(d != NULL, "opendir() failed");
	while ((e = readdir(d)) != NULL) {
		if (strcmp(e->d_name, fname) == 0)
			sawf = 1;
		if (strcmp(e->d_name, dname) == 0)
			sawd = 1;
	}
	(void)closedir(d);
	pc_check(sawf && sawd, "readdir() did not return the %ld-byte names unchanged", nm);

	if (notrunc != 0) {
		fd = open(over, O_WRONLY | O_CREAT | O_EXCL, 0600);
		pc_expect_err(fd, ENAMETOOLONG, "open() of a name one byte over {NAME_MAX}");
		pc_expect_err(mkdir(over, 0700), ENAMETOOLONG, "mkdir() of a name one byte over {NAME_MAX}");
		pc_expect_err(stat(over, &st), ENAMETOOLONG, "stat() of a name one byte over {NAME_MAX}");
	}
	pc_check(unlink(fname) == 0, "unlink() of the long name failed");
	pc_check(rmdir(dname) == 0, "rmdir() of the long name failed");
	pc_pass();
}
