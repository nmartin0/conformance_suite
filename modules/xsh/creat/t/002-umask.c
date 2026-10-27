/*
 * The mode of a created file is the mode argument less the umask.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.creat.002
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
	pc_begin("XSH.creat.002");
	pc_require_space();
	(void)umask(027);
	pc_test();
	pc_expect_ok(creat("f", 0777), "creat(\"f\", 0777)");
	pc_check(pc_file_mode("f") == 0750, "mode is %o, not 750 (mask 027)", pc_file_mode("f"));
	pc_pass();
}
