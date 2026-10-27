/*
 * strlcpy: no overwrite.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strlcpy.016
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
static size_t (*volatile f_strlcpy)(char *restrict, const char *restrict, size_t) = strlcpy;
static size_t (*volatile f_strlcat)(char *restrict, const char *restrict, size_t) = strlcat;

int main(void)
{
	unsigned char *g1, *g2, *d, *s;
	pc_begin("XSH.strlcpy.016");
	g1 = guard_page();
	g2 = guard_page();
	d = g1 - 5; s = g2 - 6;
	memcpy(s, "cdefg\0", 6);
	memset(d, 'X', 5);
	pc_test();
	pc_check(f_strlcpy((char *)d + 1, "abcdef", 4) == 6, "strlcpy(): wrong return value");
	pc_check(memcmp(d, "Xabc\0", 5) == 0, "strlcpy(): wrong destination");
	memcpy(d, "ab\0XX", 5);
	pc_check(f_strlcat((char *)d, (const char *)s, 5) == 7, "strlcat(): wrong return value");
	pc_check(memcmp(d, "abcd\0", 5) == 0, "strlcat(): wrong destination");
	pc_pass();
}
