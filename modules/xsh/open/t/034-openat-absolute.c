/*
 * openat() with an absolute path ignores its descriptor.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.034
 * MUTANT    open.at_abs_uses_fd
 */
#include "pc.h"
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	char path[4096];
	int dfd, fd;
	char c;

	pc_begin("XSH.open.034");
	pc_make_dir("d");
	pc_make_file("f", "C");
	pc_check(getcwd(path, sizeof path - 4) != NULL, "getcwd failed");
	strcat(path, "/f");
	dfd = open("d", O_RDONLY | O_DIRECTORY);
	pc_check(dfd >= 0, "cannot open directory d");
	pc_test();
	fd = openat(dfd, path, O_RDONLY);
	pc_expect_ok(fd, "openat(dfd, absolute path, O_RDONLY)");
	pc_check(read(fd, &c, 1) == 1 && c == 'C', "opened the wrong file");
	pc_pass();
}
