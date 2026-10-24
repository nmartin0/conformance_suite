/*
 * O_CREAT on a directory gives EISDIR.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.039
 * MUTANT    open.creat_on_dir_ok
 * VARIANT   open.accmode_bits
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	pc_begin("XSH.open.039");
	pc_make_dir("d");
	pc_test();
	pc_expect_err(open("d", O_RDONLY | O_CREAT, 0600), EISDIR,
	    "open(\"d\", O_RDONLY|O_CREAT) on a directory");
	pc_pass();
}
