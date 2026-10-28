/*
 * The root directory, /dev and /tmp exist and are directories; /dev holds
 * /dev/null and /dev/tty.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.10.1-directory-structure-and-files.001
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(void)
{
	static const char *dirs[] = { "/", "/dev", "/tmp" };
	static const char *files[] = { "/dev/null", "/dev/tty" };
	struct stat sb;
	size_t i;

	pc_begin("XBD.10.1-directory-structure-and-files.001");
	pc_test();
	for (i = 0; i < sizeof dirs / sizeof dirs[0]; i++) {
		pc_check(stat(dirs[i], &sb) == 0, "stat(\"%s\") failed: %s", dirs[i], pc_errname(errno));
		pc_check(S_ISDIR(sb.st_mode), "\"%s\" is not a directory", dirs[i]);
	}
	for (i = 0; i < sizeof files / sizeof files[0]; i++)
		pc_check(stat(files[i], &sb) == 0, "stat(\"%s\") failed: %s", files[i], pc_errname(errno));
	pc_pass();
}
