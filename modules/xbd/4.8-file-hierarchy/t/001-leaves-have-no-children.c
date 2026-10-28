/*
 * Only directories have entries below them: a pathname that goes through a
 * regular file, a FIFO or a symbolic link to a file cannot be used to
 * create anything, while the same names below a directory can.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.8-file-hierarchy.001
 */
#include "pc.h"
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

static int exists(const char *p)
{
	struct stat sb;

	return lstat(p, &sb) == 0;
}

int main(void)
{
	int fd, i;
	static const char *leaf[] = { "reg", "fifo", "sl" };

	pc_begin("XBD.4.8-file-hierarchy.001");
	pc_make_file("reg", "x");
	if (mkfifo("fifo", 0600) != 0)
		pc_unresolved("mkfifo failed: %s", pc_errname(errno));
	pc_make_symlink("reg", "sl");
	pc_make_dir("dir");
	pc_test();
	/* below a directory the same operations work */
	fd = open("dir/new", O_WRONLY | O_CREAT | O_EXCL, 0600);
	pc_check(fd >= 0, "open(O_CREAT) below a directory failed: %s", pc_errname(errno));
	(void)close(fd);
	pc_check(mkdir("dir/sub", 0700) == 0, "mkdir below a directory failed: %s", pc_errname(errno));
	for (i = 0; i < 3; i++) {
		char p[32];

		(void)snprintf(p, sizeof p, "%s/new", leaf[i]);
		fd = open(p, O_WRONLY | O_CREAT | O_EXCL, 0600);
		pc_check(fd < 0, "open(\"%s\", O_CREAT) succeeded, but %s is not a directory", p, leaf[i]);
		pc_check(errno == ENOTDIR, "open(\"%s\", O_CREAT) failed with %s, expected ENOTDIR", p, pc_errname(errno));
		(void)snprintf(p, sizeof p, "%s/sub", leaf[i]);
		pc_check(mkdir(p, 0700) == -1 && errno == ENOTDIR, "mkdir(\"%s\") did not fail with ENOTDIR (%s)", p, pc_errname(errno));
		(void)snprintf(p, sizeof p, "%s/lnk", leaf[i]);
		pc_check(symlink("x", p) == -1 && errno == ENOTDIR, "symlink(\"x\", \"%s\") did not fail with ENOTDIR (%s)", p, pc_errname(errno));
		(void)snprintf(p, sizeof p, "%s/fifo2", leaf[i]);
		pc_check(mkfifo(p, 0600) == -1 && errno == ENOTDIR, "mkfifo(\"%s\") did not fail with ENOTDIR (%s)", p, pc_errname(errno));
	}
	pc_check(!exists("reg/new") && !exists("reg/sub"), "something was created below the regular file");
	pc_pass();
}
