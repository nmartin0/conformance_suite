/*
 * openat() with O_CREAT applies the umask.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.037
 * MUTANT    open.at_mode_ignores_umask
 * VARIANT   open.accmode_bits
 */
#include "pc.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void)
{
	int dfd, fd;

	pc_begin("XSH.open.037");
	pc_make_dir("d");
	pc_require_space();
	dfd = open("d", O_RDONLY | O_DIRECTORY);
	pc_check(dfd >= 0, "cannot open directory d");
	(void)umask(027);
	pc_test();
	fd = openat(dfd, "n", O_WRONLY | O_CREAT | O_EXCL, 0666);
	pc_expect_ok(fd, "openat(dfd, \"n\", O_WRONLY|O_CREAT|O_EXCL, 0666)");
	pc_check(pc_file_mode("d/n") == 0640, "mode is %04o, expected 0640 (0666 less umask 027)",
	    pc_file_mode("d/n"));
	pc_pass();
}
