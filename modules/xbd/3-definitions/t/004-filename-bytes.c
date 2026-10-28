/*
 * A name of {NAME_MAX} bytes works, and no name read from a directory holds
 * a slash.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.3-definitions.004
 */
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "pc.h"

int main(void)
{
	char name[1002];
	struct stat st;
	struct dirent *e;
	DIR *d;
	long nm;
	int fd, found = 0;

	pc_begin("XBD.3-definitions.004");
	errno = 0;
	nm = pathconf(".", _PC_NAME_MAX);
	if (nm < 0)
		pc_notinuse("pathconf(_PC_NAME_MAX) reports no limit");
	if (nm > 1000)
		pc_notinuse("{NAME_MAX} of %ld is above the 1000 this test builds", nm);
	pc_check(nm >= 1, "pathconf(_PC_NAME_MAX) is %ld", nm);
	memset(name, 'a', (size_t)nm);
	name[nm] = '\0';
	pc_require_space();
	pc_test();
	fd = open(name, O_CREAT | O_EXCL | O_WRONLY, 0600);
	pc_check(fd >= 0, "creating a file whose name has {NAME_MAX} = %ld "
	    "bytes failed: %s", nm, pc_errname(errno));
	close(fd);
	pc_check(stat(name, &st) == 0, "stat of the new file failed: %s",
	    pc_errname(errno));
	fd = open("nosuchdir/x", O_CREAT | O_WRONLY, 0600);
	if (fd >= 0) {
		close(fd);
		unlink("nosuchdir/x");
		pc_fail("open(\"nosuchdir/x\", O_CREAT) succeeded with no "
		    "directory nosuchdir");
	}
	d = opendir(".");
	pc_check(d != NULL, "opendir(\".\") failed: %s", pc_errname(errno));
	while ((e = readdir(d)) != NULL) {
		if (strchr(e->d_name, '/') != NULL) {
			closedir(d);
			pc_fail("readdir returned the name '%s', which holds a "
			    "slash", e->d_name);
		}
		if (strcmp(e->d_name, name) == 0)
			found++;
	}
	closedir(d);
	unlink(name);
	pc_check(found == 1, "the directory listed the {NAME_MAX}-byte name "
	    "%d times, expected once", found);
	pc_pass();
}
