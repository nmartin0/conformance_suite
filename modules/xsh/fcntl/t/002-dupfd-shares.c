/*
 * F_DUPFD shares the open file description.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fcntl.002
 * MUTANT    fcntl.dupfd_fresh
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd, d, fl;
	char c;

	pc_begin("XSH.fcntl.002");
	pc_make_file("f", "abc");
	fd = open("f", O_RDONLY);
	pc_check(fd >= 0, "cannot open f");
	pc_test();
	d = fcntl(fd, F_DUPFD, 0);
	pc_expect_ok(d, "fcntl(F_DUPFD, 0)");
	pc_check(read(fd, &c, 1) == 1 && c == 'a', "read on the original failed");
	pc_check(read(d, &c, 1) == 1 && c == 'b', "the offset is not shared");
	fl = fcntl(d, F_GETFL);
	pc_check(fl != -1 && fcntl(d, F_SETFL, fl | O_APPEND) == 0, "F_SETFL failed");
	fl = fcntl(fd, F_GETFL);
	pc_check(fl != -1 && (fl & O_APPEND), "O_APPEND set on the duplicate not seen on the original");
	pc_pass();
}
