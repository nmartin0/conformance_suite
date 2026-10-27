/*
 * For a file opened with each access mode, with and without O_APPEND and
 * O_NONBLOCK, the F_GETFL result masked with O_ACCMODE equals the access
 * mode.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fcntl.h.014
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	static const int modes[3] = { O_RDONLY, O_WRONLY, O_RDWR };
	static const char *const mn[3] = { "O_RDONLY", "O_WRONLY", "O_RDWR" };
	static const int extra[4] = { 0, O_APPEND, O_NONBLOCK, O_APPEND | O_NONBLOCK };
	int k, x;

	pc_begin("XBD.fcntl.h.014");
	pc_make_file("f", "x");
	pc_test();
	for (k = 0; k < 3; k++)
		for (x = 0; x < 4; x++) {
			int fd = open("f", modes[k] | extra[x]);
			int fl;

			pc_check(fd != -1, "open(f, %s | %#x) failed (%s)", mn[k], (unsigned)extra[x], pc_errname(errno));
			fl = fcntl(fd, F_GETFL);
			pc_check(fl != -1, "fcntl(F_GETFL) failed (%s)", pc_errname(errno));
			pc_check((fl & O_ACCMODE) == modes[k], "F_GETFL & O_ACCMODE is %#x for a descriptor opened with %s | %#x",
			    (unsigned)(fl & O_ACCMODE), mn[k], (unsigned)extra[x]);
			close(fd);
		}
	pc_pass();
}
