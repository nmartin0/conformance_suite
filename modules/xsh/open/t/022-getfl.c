/*
 * F_GETFL shows the access mode and status flags open() was given.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.022
 * MUTANT    open.wronly_readable
 * MUTANT    open.rdonly_writable
 * MUTANT    open.append_ignored
 * MUTANT    open.nonblock_ignored
 */
#include "pc.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void)
{
	/*
	 * O_NONBLOCK is checked on a FIFO: for a regular file it is
	 * unspecified whether F_GETFL shows it.
	 */
	static const struct { const char *path; int flags; const char *name; } c[] = {
		{ "f", O_RDONLY, "O_RDONLY" },
		{ "f", O_WRONLY, "O_WRONLY" },
		{ "f", O_RDWR, "O_RDWR" },
		{ "f", O_WRONLY | O_APPEND, "O_WRONLY|O_APPEND" },
		{ "p", O_WRONLY | O_NONBLOCK, "FIFO O_WRONLY|O_NONBLOCK" },
	};
	size_t i;
	int fd, fl, want, r;

	pc_begin("XSH.open.022");
	pc_make_file("f", "x");
	pc_check(mkfifo("p", 0600) == 0, "mkfifo failed");
	r = open("p", O_RDONLY | O_NONBLOCK);	/* a reader, so the writer opens */
	pc_check(r >= 0, "cannot open the FIFO for reading");
	pc_test();
	for (i = 0; i < sizeof c / sizeof c[0]; i++) {
		fd = open(c[i].path, c[i].flags);
		pc_expect_ok(fd, c[i].name);
		fl = fcntl(fd, F_GETFL);
		pc_expect_ok(fl, "fcntl(F_GETFL)");
		want = c[i].flags & O_ACCMODE;
		pc_check((fl & O_ACCMODE) == want,
		    "%s: F_GETFL access mode is %#x, expected %#x",
		    c[i].name, fl & O_ACCMODE, want);
		pc_check(!(c[i].flags & O_APPEND) || (fl & O_APPEND),
		    "%s: O_APPEND not shown by F_GETFL", c[i].name);
		pc_check(!(c[i].flags & O_NONBLOCK) || (fl & O_NONBLOCK),
		    "%s: O_NONBLOCK not shown by F_GETFL", c[i].name);
		(void)close(fd);
	}
	pc_pass();
}
