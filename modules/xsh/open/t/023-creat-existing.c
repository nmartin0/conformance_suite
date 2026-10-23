/*
 * O_CREAT on an existing file leaves its contents and mode alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.023
 * MUTANT    open.creat_truncates_existing
 * MUTANT    open.creat_resets_mode
 */
#include "pc.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void)
{
	int fd;

	pc_begin("XSH.open.023");
	pc_make_file("f", "keep");
	pc_check(chmod("f", 0640) == 0, "chmod failed");
	pc_test();
	fd = open("f", O_WRONLY | O_CREAT, 0600);
	pc_expect_ok(fd, "open(\"f\", O_WRONLY|O_CREAT)");
	pc_check(pc_file_equals("f", "keep"), "existing file's contents changed");
	pc_check(pc_file_mode("f") == 0640, "mode changed from 0640 to %04o",
	    pc_file_mode("f"));
	pc_pass();
}
