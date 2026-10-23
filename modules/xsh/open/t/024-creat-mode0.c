/*
 * A file created with mode 0 is still readable and writable through the creating descriptor.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.024
 * MUTANT    open.mode0_reopens_rdonly
 */
#include "pc.h"
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
	int fd;
	char c;

	pc_begin("XSH.open.024");
	pc_require_space();
	pc_test();
	fd = open("f", O_RDWR | O_CREAT | O_EXCL, 0);
	pc_expect_ok(fd, "open(\"f\", O_RDWR|O_CREAT|O_EXCL, 0)");
	pc_check(write(fd, "z", 1) == 1, "write() on the new descriptor failed");
	pc_check(lseek(fd, 0, SEEK_SET) == 0, "lseek() failed");
	pc_check(read(fd, &c, 1) == 1 && c == 'z', "read() on the new descriptor failed");
	pc_pass();
}
