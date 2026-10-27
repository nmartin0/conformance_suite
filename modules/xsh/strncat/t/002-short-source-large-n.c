/*
 * strncat: short source large n.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strncat.002
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
static char *(*volatile f_strncat)(char *restrict, const char *restrict, size_t) = strncat;

int main(void)
{
	unsigned char *g, *d;
	pc_begin("XSH.strncat.002");
	g = guard_page();
	d = g - 4;
	memcpy(d, "ab\0\0", 4);
	pc_test();
	pc_check(f_strncat((char *)d, "c", 1000) == (char *)d, "the return value was not the destination");
	pc_check(memcmp(d, "abc", 4) == 0, "the result is not \"abc\"");
	pc_pass();
}
