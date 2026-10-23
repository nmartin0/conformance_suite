/*
 * A test starts with no descriptor above 2 open (pcexec closes them).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION ST.c.015
 */
#include "pc.h"
#include <fcntl.h>

int main(void)
{
	int fd, open_fd = -1;

	for (fd = 3; fd < 1024; fd++)
		if (fcntl(fd, F_GETFD) != -1) {
			open_fd = fd;
			break;
		}
	pc_begin("ST.c.015");
	pc_test();
	pc_check(open_fd == -1, "descriptor %d was open at start", open_fd);
	pc_pass();
}
