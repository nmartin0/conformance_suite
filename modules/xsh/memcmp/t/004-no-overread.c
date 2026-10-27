/*
 * memcmp: no overread.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memcmp.004
 */
#include "pc.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

/* Setup: a writable page followed by an inaccessible one; returns the address where the inaccessible page begins. */
static unsigned char *guard_page(void)
{
	long ps = sysconf(_SC_PAGESIZE);
	unsigned char *p;

	pc_check(ps > 0, "sysconf(_SC_PAGESIZE) failed");
#ifdef MAP_ANONYMOUS
	p = mmap(NULL, (size_t)ps * 2, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#else
	{
		int fd = open("pages", O_RDWR | O_CREAT | O_TRUNC, 0600);

		pc_check(fd >= 0, "open(pages) failed: %s", pc_errname(errno));
		pc_check(ftruncate(fd, (off_t)ps * 2) == 0, "ftruncate failed: %s", pc_errname(errno));
		p = mmap(NULL, (size_t)ps * 2, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
		(void)close(fd);
	}
#endif
	pc_check(p != MAP_FAILED, "mmap failed: %s", pc_errname(errno));
	pc_check(mprotect(p + ps, (size_t)ps, PROT_NONE) == 0, "mprotect failed: %s", pc_errname(errno));
	return p + ps;
}
static int (*volatile f_memcmp)(const void *, const void *, size_t) = memcmp;

int main(void)
{
	unsigned char *g, *p, *q;
	pc_begin("XSH.memcmp.004");
	g = guard_page();
	p = g - 4; q = g - 4;
	p[0] = 1; p[1] = 2; p[2] = 3; p[3] = 4;
	pc_test();
	pc_check(f_memcmp(p, q, 4) == 0, "the same array did not compare equal");
	q = g - 8;
	memcpy(q, p, 4);
	pc_check(f_memcmp(p, q, 4) == 0, "equal arrays at the boundary did not compare equal");
	q[3] = 9;
	pc_check(f_memcmp(p, q, 4) < 0, "differing arrays at the boundary were not ordered");
	pc_check(f_memcmp(g, g, 0) == 0, "n 0 on the boundary did not compare equal");
	pc_pass();
}
