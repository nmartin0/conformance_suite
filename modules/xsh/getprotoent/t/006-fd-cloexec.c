/*
 * A descriptor held open for the protocols database after the open call with a non-zero argument has FD_CLOEXEC set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getprotoent.006
 */
#include "dbcommon.h"

int main(void)
{
	unsigned char before[FDMAX], after[FDMAX];
	int fd, found = 0;

	pc_begin("XSH.getprotoent.006");
	snapshot(before);
	pc_test();
	SETENT(1);
	(void)GETENT();
	snapshot(after);
	for (fd = 0; fd < FDMAX; fd++)
		if (after[fd] && !before[fd]) {
			int fl = fcntl(fd, F_GETFD);

			found = 1;
			pc_check(fl != -1 && (fl & FD_CLOEXEC) != 0, "descriptor %d, opened for the database, lacks FD_CLOEXEC", fd);
		}
	ENDENT();
	if (!found)
		pc_notinuse("the implementation holds no descriptor for the database");
	pc_pass();
}
