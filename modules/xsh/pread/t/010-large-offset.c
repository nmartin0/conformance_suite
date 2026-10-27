/*
 * pread() at an offset beyond 32 bits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pread.010
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int main(void)
{
	off_t off;
	char c = 0;
	int fd;

	pc_begin("XSH.pread.010");
	if (sizeof(off_t) < 8)
		pc_notinuse("off_t has fewer than 64 bits");
	off = (off_t)1 << 32;
	off += 5;
	pc_require_space();
	(void)signal(SIGXFSZ, SIG_IGN);	/* a lowered file size limit gives EFBIG, not a signal */
	fd = open("f", O_RDWR | O_CREAT, 0600);
	pc_check(fd >= 0, "cannot create f");
	if (lseek(fd, off, SEEK_SET) != off)
		pc_unresolved("lseek to %lld failed: %s", (long long)off, pc_errname(errno));
	if (write(fd, "Z", 1) != 1)
		pc_unresolved("write at %lld failed: %s", (long long)off, pc_errname(errno));
	pc_test();
	pc_check(pread(fd, &c, 1, off) == 1, "pread() at %lld did not return 1", (long long)off);
	pc_check(c == 'Z', "pread() at %lld read %d, not the byte written there", (long long)off, c);
	c = 'q';
	pc_check(pread(fd, &c, 1, 5) == 1, "pread() at 5 did not return 1");
	pc_check(c == 0, "pread() at 5 read %d: the offset was truncated to 32 bits", c);
	pc_pass();
}
