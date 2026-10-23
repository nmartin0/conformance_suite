/*
 * read() on a closed or write-only descriptor gives EBADF.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.read.012
 * MUTANT    read.ebadf_as_einval
 * MUTANT    read.wronly_ok
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	int bad, w;
	char c;

	pc_begin("XSH.read.012");
	pc_make_file("f", "x");
	w = open("f", O_WRONLY);
	pc_check(w >= 0, "cannot open f");
	bad = pc_lowest_free_fd();
	pc_test();
	pc_expect_err(read(bad, &c, 1), EBADF, "read(<not open>)");
	pc_expect_err(read(w, &c, 1), EBADF, "read(<write-only descriptor>)");
	pc_pass();
}
