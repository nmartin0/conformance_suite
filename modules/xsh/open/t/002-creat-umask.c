/*
 * O_CREAT sets permission bits to mode & ~umask.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.002
 * MUTANT    open.mode_ignores_umask
 * MUTANT    open.mode_ignored
 * VARIANT   open.accmode_bits
 *
 * Two masks are tried so a coincidence cannot pass.
 */
#include "pc.h"
#include <fcntl.h>
#include <sys/stat.h>

static void one(const char *name, mode_t mask, mode_t mode, unsigned want)
{
	int fd;
	unsigned got;

	(void)umask(mask);
	fd = open(name, O_WRONLY | O_CREAT | O_EXCL, mode);
	pc_expect_ok(fd, "open with O_CREAT");
	got = pc_file_mode(name) & 0777;
	pc_check(got == want, "umask %03o, mode %03o: file mode %03o, "
	    "expected %03o", (unsigned)mask, (unsigned)mode, got, want);
}

int main(void)
{
	pc_begin("XSH.open.002");
	pc_require_space();
	pc_test();
	one("a", 022, 0666, 0644);
	one("b", 077, 0751, 0700);
	pc_pass();
}
