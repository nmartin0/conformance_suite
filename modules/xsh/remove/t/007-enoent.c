/*
 * remove() of a missing name, through a missing directory, or of "" fails with ENOENT.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.remove.007
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
	pc_begin("XSH.remove.007");
	pc_test();
	pc_expect_err(remove("missing"), ENOENT, "remove(\"missing\")");
	pc_expect_err(remove("nodir/x"), ENOENT, "remove(\"nodir/x\")");
	pc_expect_err(remove(""), ENOENT, "remove(\"\")");
	pc_pass();
}
