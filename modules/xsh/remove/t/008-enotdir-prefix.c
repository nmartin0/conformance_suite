/*
 * remove("f/x") with f a regular file fails with ENOTDIR.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.remove.008
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
	pc_begin("XSH.remove.008");
	pc_make_file("f", "data");
	pc_test();
	pc_expect_err(remove("f/x"), ENOTDIR, "remove(\"f/x\")");
	pc_check(pc_file_equals("f", "data"), "f changed");
	pc_pass();
}
