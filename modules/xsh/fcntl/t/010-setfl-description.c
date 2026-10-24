/*
 * F_SETFL acts on the open file description only.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fcntl.010
 * MUTANT    fcntl.setfl_ignored
 * MUTANT    fcntl.setfl_by_file
 * VARIANT   fcntl.ok_other
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int a, b, c, fl;

	pc_begin("XSH.fcntl.010");
	pc_make_file("f", "x");
	a = open("f", O_WRONLY);
	c = open("f", O_WRONLY);
	pc_check(a >= 0 && c >= 0, "cannot open f");
	b = dup(a);
	pc_check(b >= 0, "dup failed");
	fl = fcntl(a, F_GETFL);
	pc_check(fl != -1, "F_GETFL failed");
	pc_test();
	pc_expect_ok(fcntl(a, F_SETFL, fl | O_APPEND), "fcntl(F_SETFL, O_APPEND)");
	fl = fcntl(b, F_GETFL);
	pc_check(fl != -1 && (fl & O_APPEND), "the duplicate does not see O_APPEND");
	fl = fcntl(c, F_GETFL);
	pc_check(fl != -1 && !(fl & O_APPEND), "another open file description got O_APPEND");
	pc_pass();
}
