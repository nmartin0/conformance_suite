/*
 * /dev/tty is both readable and writable.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.10.1-directory-structure-and-files.004
 */
#include "pc.h"
#include <errno.h>
#include <unistd.h>

int main(void)
{
	pc_begin("XBD.10.1-directory-structure-and-files.004");
	pc_test();
	pc_check(access("/dev/tty", R_OK) == 0, "access(\"/dev/tty\", R_OK) failed: %s", pc_errname(errno));
	pc_check(access("/dev/tty", W_OK) == 0, "access(\"/dev/tty\", W_OK) failed: %s", pc_errname(errno));
	pc_pass();
}
