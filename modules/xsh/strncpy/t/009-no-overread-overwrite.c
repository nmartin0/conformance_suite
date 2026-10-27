/*
 * strncpy: no overread overwrite.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncpy.009
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
static char *(*volatile f_strncpy)(char *restrict, const char *restrict, size_t) = strncpy;
static char *(*volatile f_stpncpy)(char *restrict, const char *restrict, size_t) = stpncpy;

int main(void)
{
	unsigned char *g1, *g2, *s, *d;
	pc_begin("XSH.strncpy.009");
	g1 = guard_page();
	g2 = guard_page();
	s = g1 - 4; d = g2 - 4;
	memcpy(s, "wxyz", 4);
	pc_test();
	memset(d, 0, 4);
	pc_check(f_strncpy((char *)d, (const char *)s, 4) == (char *)d, "strncpy() wrong return value");
	pc_check(memcmp(d, "wxyz", 4) == 0, "strncpy() wrong contents");
	memset(d, 0, 4);
	pc_check(f_stpncpy((char *)d, (const char *)s, 4) == (char *)d + 4, "stpncpy() wrong return value");
	pc_check(memcmp(d, "wxyz", 4) == 0, "stpncpy() wrong contents");
	pc_pass();
}
