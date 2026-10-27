/*
 * creat() through a symbolic link loop fails with ELOOP.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.creat.014
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
	pc_begin("XSH.creat.014");
	pc_make_symlink("b", "a");
	pc_make_symlink("a", "b");
	pc_test();
	pc_expect_err(creat("a", 0600), ELOOP, "creat(\"a\") with a->b->a");
	pc_pass();
}
