/*
 * O_TRUNC truncates an existing regular file to 0; mode unchanged.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.007
 * MUTANT    open.trunc_ignored
 * MUTANT    open.trunc_resets_mode
 */
#include "pc.h"
#include <fcntl.h>
#include <sys/stat.h>

int main(void)
{
	int fd;
	unsigned before, after;

	pc_begin("XSH.open.007");
	pc_make_file("f", "hello");
	pc_check(chmod("f", 0640) == 0, "chmod");
	before = pc_file_mode("f");
	pc_test();
	fd = open("f", O_WRONLY | O_TRUNC);
	pc_expect_ok(fd, "open(\"f\", O_WRONLY|O_TRUNC)");
	pc_check(pc_file_size("f") == 0, "size is %ld after O_TRUNC",
	    pc_file_size("f"));
	after = pc_file_mode("f");
	pc_check(after == before, "mode changed from %04o to %04o", before,
	    after);
	pc_pass();
}
