/*
 * A process with appropriate privileges may read and write any file and
 * search any directory whatever the permission bits say.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.7-file-access-permissions.005
 */
#include "pc.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define U_OTHER 65534

int main(void)
{
	int fd;
	char b[8];
	struct stat sb;
	DIR *d;

	pc_begin("XBD.4.7-file-access-permissions.005");
	if (geteuid() != 0)
		pc_unresolved("needs root");
	pc_make_file("pf", "data");
	pc_make_dir("pd");
	pc_make_file("pd/f", "x");
	if (chown("pf", U_OTHER, U_OTHER) != 0 || chown("pd", U_OTHER, U_OTHER) != 0 || chown("pd/f", U_OTHER, U_OTHER) != 0)
		pc_unresolved("chown failed: %s", pc_errname(errno));
	pc_test();
	if (chmod("pf", 0) != 0 || chmod("pd", 0) != 0)
		pc_unresolved("chmod failed: %s", pc_errname(errno));
	fd = open("pf", O_RDONLY);
	pc_check(fd >= 0, "a privileged open(O_RDONLY) of a mode 0 file failed: %s", pc_errname(errno));
	pc_check(read(fd, b, sizeof b) == 4, "read of the mode 0 file did not return its 4 bytes");
	(void)close(fd);
	fd = open("pf", O_WRONLY);
	pc_check(fd >= 0, "a privileged open(O_WRONLY) of a mode 0 file failed: %s", pc_errname(errno));
	pc_check(write(fd, "ABCD", 4) == 4, "write to the mode 0 file failed");
	(void)close(fd);
	fd = open("pf", O_RDWR);
	pc_check(fd >= 0, "a privileged open(O_RDWR) of a mode 0 file failed: %s", pc_errname(errno));
	(void)close(fd);
	pc_check(stat("pd/f", &sb) == 0, "a privileged stat() through a mode 0 directory failed: %s", pc_errname(errno));
	d = opendir("pd");
	pc_check(d != NULL, "a privileged opendir() of a mode 0 directory failed: %s", pc_errname(errno));
	(void)closedir(d);
	pc_check(chdir("pd") == 0, "a privileged chdir() into a mode 0 directory failed: %s", pc_errname(errno));
	fd = open("f", O_RDWR);
	pc_check(fd >= 0, "a privileged open() of a file in a mode 0 directory (after chdir) failed: %s", pc_errname(errno));
	(void)close(fd);
	pc_pass();
}
