/*
 * {NAME_MAX} of a directory applies to the filenames in it, and with
 * _POSIX_NO_TRUNC in effect a longer name is an error.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fpathconf.011
 * MUTANT    fpathconf.name_max_plus_one
 * MUTANT    fpathconf.truncates_long_names
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	char name[4097];
	long n, nt;
	int fd;

	pc_begin("XSH.fpathconf.011");
	pc_make_dir("d");
	pc_check(chdir("d") == 0, "chdir failed: %s", pc_errname(errno));
	pc_require_space();
	pc_test();
	n = pathconf(".", _PC_NAME_MAX);
	if (n == -1 || n >= 4096)
		pc_notinuse("pathconf(\".\", _PC_NAME_MAX) is %ld: no filename length to try", n);
	nt = pathconf(".", _PC_NO_TRUNC);
	memset(name, 'n', (size_t)n + 1);
	name[n] = '\0';
	fd = open(name, O_WRONLY | O_CREAT | O_EXCL, 0600);
	pc_check(fd >= 0, "creating a file with a %ld-byte name ({NAME_MAX}) failed: %s", n, pc_errname(errno));
	(void)close(fd);
	(void)unlink(name);
	if (nt == -1) {
		pc_note("_PC_NO_TRUNC is -1: longer names are not checked");
		pc_pass();
	}
	name[n] = 'n';
	name[n + 1] = '\0';
	errno = 0;
	fd = open(name, O_WRONLY | O_CREAT | O_EXCL, 0600);
	pc_check(fd == -1, "creating a file with a %ld-byte name (above {NAME_MAX}) succeeded under _POSIX_NO_TRUNC",
		 n + 1);
	pc_check(errno == ENAMETOOLONG, "creating a file with a %ld-byte name failed with %s, expected ENAMETOOLONG",
		 n + 1, pc_errname(errno));
	pc_pass();
}
