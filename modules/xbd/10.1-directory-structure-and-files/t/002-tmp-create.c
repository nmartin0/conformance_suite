/*
 * An application may create files (and directories) in /tmp, write them,
 * read them back and remove them.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.10.1-directory-structure-and-files.002
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

static char fpath[64], dpath[64], ipath[96];

static void cleanup(void)
{
	if (ipath[0] != '\0')
		(void)unlink(ipath);
	if (dpath[0] != '\0')
		(void)rmdir(dpath);
	if (fpath[0] != '\0')
		(void)unlink(fpath);
}

int main(void)
{
	int fd;
	char buf[16];
	struct stat sb;

	pc_begin("XBD.10.1-directory-structure-and-files.002");
	(void)atexit(cleanup);
	pc_test();
	(void)strcpy(fpath, "/tmp/pcXXXXXX");
	fd = mkstemp(fpath);
	if (fd < 0) {
		fpath[0] = '\0';
		pc_fail("a file cannot be created in /tmp: %s", pc_errname(errno));
	}
	pc_check(write(fd, "temporary", 9) == 9, "write to the file in /tmp failed: %s", pc_errname(errno));
	pc_check(lseek(fd, 0, SEEK_SET) == 0, "lseek failed");
	pc_check(read(fd, buf, sizeof buf) == 9 && memcmp(buf, "temporary", 9) == 0, "the data written to the file in /tmp is not read back");
	pc_check(close(fd) == 0, "close failed");
	(void)strcpy(dpath, "/tmp/pcdXXXXXX");
	if (mkdtemp(dpath) == NULL) {
		dpath[0] = '\0';
		pc_fail("a directory cannot be created in /tmp: %s", pc_errname(errno));
	}
	(void)strcpy(ipath, dpath);
	(void)strcat(ipath, "/inner");
	fd = open(ipath, O_WRONLY | O_CREAT | O_EXCL, 0600);
	pc_check(fd >= 0, "a file cannot be created in a directory made in /tmp: %s", pc_errname(errno));
	(void)close(fd);
	pc_check(unlink(ipath) == 0, "unlink of the inner file failed: %s", pc_errname(errno));
	ipath[0] = '\0';
	pc_check(rmdir(dpath) == 0, "rmdir of the directory in /tmp failed: %s", pc_errname(errno));
	dpath[0] = '\0';
	pc_check(unlink(fpath) == 0, "unlink of the file in /tmp failed: %s", pc_errname(errno));
	pc_check(stat(fpath, &sb) == -1 && errno == ENOENT, "the file in /tmp is still there after unlink");
	fpath[0] = '\0';
	pc_pass();
}
