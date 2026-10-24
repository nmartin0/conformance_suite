/*
 * A component longer than {NAME_MAX} fails with ENAMETOOLONG.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.017
 * MUTANT    open.enametoolong_as_enoent
 * VARIANT   open.accmode_bits
 *
 * {NAME_MAX} comes from pathconf() on the directory, since it can differ
 * between file systems. O_CREAT is set in an existing directory so
 * ENOENT cannot also apply.
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	long max;
	char *name;
	int fd;

	pc_begin("XSH.open.017");
	errno = 0;
	max = pathconf(".", _PC_NAME_MAX);
	if (max == -1 && errno == 0)
		pc_notinuse("{NAME_MAX} is unbounded for this directory");
	pc_check(max > 0, "pathconf(_PC_NAME_MAX) failed");
	name = malloc((size_t)max + 2);
	pc_check(name != NULL, "malloc");
	memset(name, 'a', (size_t)max + 1);
	name[max + 1] = '\0';
	pc_require_space();
	pc_test();
	fd = open(name, O_WRONLY | O_CREAT, 0600);
	pc_expect_err(fd, ENAMETOOLONG, "open(<NAME_MAX+1 bytes>, O_CREAT)");
	pc_pass();
}
