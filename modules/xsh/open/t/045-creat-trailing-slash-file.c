/*
 * O_CREAT on "file/" fails with ENOTDIR and leaves the file alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.045
 * MUTANT    open.trailing_slash_ignored
 * MUTANT    open.enotdir_as_enoent
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	pc_begin("XSH.open.045");
	pc_make_file("f", "keep");
	pc_test();
	pc_expect_err(open("f/", O_WRONLY | O_CREAT | O_TRUNC, 0600), ENOTDIR,
	    "open(\"f/\", O_WRONLY|O_CREAT|O_TRUNC)");
	pc_check(pc_file_equals("f", "keep"), "file was modified");
	pc_pass();
}
