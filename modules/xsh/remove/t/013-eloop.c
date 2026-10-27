/*
 * remove() through a symbolic link loop fails with ELOOP.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.remove.013
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
	pc_begin("XSH.remove.013");
	pc_make_symlink("b", "a");
	pc_make_symlink("a", "b");
	pc_test();
	pc_expect_err(remove("a/x"), ELOOP, "remove(\"a/x\") with a->b->a");
	pc_pass();
}
