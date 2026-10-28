/*
 * /dev/console exists (it need not be readable or writable).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.10.1-directory-structure-and-files.006
 */
#include "pc.h"
#include <errno.h>
#include <sys/stat.h>

int main(void)
{
	struct stat sb;

	pc_begin("XBD.10.1-directory-structure-and-files.006");
	pc_test();
	pc_check(stat("/dev/console", &sb) == 0, "stat(\"/dev/console\") failed: %s", pc_errname(errno));
	pc_pass();
}
