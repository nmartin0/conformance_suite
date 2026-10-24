/*
 * A symbolic link loop gives ELOOP.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.038
 * MUTANT    open.eloop_as_enoent
 * VARIANT   open.accmode_bits
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	pc_begin("XSH.open.038");
	pc_make_symlink("b", "a");
	pc_make_symlink("a", "b");
	pc_test();
	pc_expect_err(open("a", O_RDONLY), ELOOP, "open(\"a\") with a->b->a");
	pc_pass();
}
