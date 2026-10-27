/*
 * The standard streams are open at start.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.stdin.002
 */
#include "pc.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	int fl0, fl1, fl2;

	pc_begin("XSH.stdin.002");
	pc_test();
	pc_check(stdin != NULL && stdout != NULL && stderr != NULL, "a standard stream pointer is null");
	fl0 = fcntl(0, F_GETFL);
	fl1 = fcntl(1, F_GETFL);
	fl2 = fcntl(2, F_GETFL);
	pc_check(fl0 != -1, "descriptor 0 is not open");
	pc_check(fl1 != -1, "descriptor 1 is not open");
	pc_check(fl2 != -1, "descriptor 2 is not open");
	pc_check((fl0 & O_ACCMODE) == O_RDONLY || (fl0 & O_ACCMODE) == O_RDWR, "descriptor 0 cannot be read");
	pc_check((fl1 & O_ACCMODE) == O_WRONLY || (fl1 & O_ACCMODE) == O_RDWR, "descriptor 1 cannot be written");
	pc_check((fl2 & O_ACCMODE) == O_WRONLY || (fl2 & O_ACCMODE) == O_RDWR, "descriptor 2 cannot be written");
	pc_pass();
}
