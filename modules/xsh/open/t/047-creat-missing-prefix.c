/*
 * O_CREAT with a missing directory in the prefix gives ENOENT.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.047
 * MUTANT    open.enoent_as_eacces
 * MUTANT    open.creat_makes_prefix
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void)
{
	struct stat st;

	pc_begin("XSH.open.047");
	pc_require_space();
	pc_test();
	pc_expect_err(open("m/n", O_WRONLY | O_CREAT, 0600), ENOENT,
	    "open(\"m/n\", O_WRONLY|O_CREAT) with m missing");
	pc_check(lstat("m", &st) != 0, "\"m\" was created");
	pc_pass();
}
