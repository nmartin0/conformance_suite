/*
 * pwrite() at an offset beyond 32 bits.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pwrite.011
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
	struct stat st;
	off_t off;
	char c = 0;
	ssize_t r;
	int fd, e;

	pc_begin("XSH.pwrite.011");
	if (sizeof(off_t) < 8)
		pc_notinuse("off_t has fewer than 64 bits");
	off = (off_t)1 << 32;
	off += 5;
	pc_require_space();
#ifdef SIGXFSZ
	(void)signal(SIGXFSZ, SIG_IGN);	/* a lowered file size limit gives EFBIG, not a signal */
#endif
	fd = open("f", O_RDWR | O_CREAT, 0600);
	pc_check(fd >= 0, "cannot create f");
	pc_test();
	r = pwrite(fd, "Z", 1, off);
	e = errno;
	if (r == -1 && (e == EFBIG || e == ENOSPC))
		pc_unresolved("the file system does not hold a file of %lld bytes (%s)", (long long)off + 1, pc_errname(e));
	pc_check(r == 1, "pwrite() at %lld returned %ld", (long long)off, (long)r);
	pc_check(fstat(fd, &st) == 0, "fstat failed");
	pc_check(st.st_size == off + 1, "the length is %lld, not %lld", (long long)st.st_size, (long long)off + 1);
	pc_check(pread(fd, &c, 1, off) == 1 && c == 'Z', "the byte is not at offset %lld", (long long)off);
	c = 'q';
	pc_check(pread(fd, &c, 1, 5) == 1 && c == 0, "offset 5 was written or truncated to 32 bits");
	pc_pass();
}
