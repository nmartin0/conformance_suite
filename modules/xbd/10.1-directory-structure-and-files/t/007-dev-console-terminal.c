/*
 * If /dev/console can be opened, it provides a terminal interface: it is a
 * terminal and its attributes can be read.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.10.1-directory-structure-and-files.007
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

int main(void)
{
	int fd, e;
	struct termios t;

	pc_begin("XBD.10.1-directory-structure-and-files.007");
	fd = open("/dev/console", O_RDONLY | O_NOCTTY | O_NONBLOCK);
	if (fd < 0) {
		e = errno;
		pc_unresolved("/dev/console cannot be opened here (%s); it need not be readable", pc_errname(e));
	}
	pc_test();
	pc_check(isatty(fd) == 1, "/dev/console is open but is not a terminal");
	pc_check(tcgetattr(fd, &t) == 0, "tcgetattr on /dev/console failed: %s", pc_errname(errno));
	(void)close(fd);
	pc_pass();
}
