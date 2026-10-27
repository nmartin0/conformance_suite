/*
 * The hosts end call closes the descriptor the database was holding.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gethostent.004
 */
#include "dbcommon.h"

int main(void)
{
	unsigned char before[FDMAX], after[FDMAX];
	int fd, found = 0;

	pc_begin("XSH.gethostent.004");
	snapshot(before);
	pc_test();
	SETENT(1);
	(void)GETENT();
	snapshot(after);
	ENDENT();
	for (fd = 0; fd < FDMAX; fd++)
		if (after[fd] && !before[fd]) {
			found = 1;
			pc_check(fcntl(fd, F_GETFD) == -1, "descriptor %d, opened for the database, is still open after the end call", fd);
		}
	if (!found)
		pc_notinuse("the implementation holds no descriptor for the database");
	pc_pass();
}
