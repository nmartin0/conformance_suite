/*
 * memcpy: no overread.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memcpy.005
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
static void *(*volatile f_memcpy)(void *restrict, const void *restrict, size_t) = memcpy;

int main(void)
{
	unsigned char *g, *s, *d;
	int i;
	pc_begin("XSH.memcpy.005");
	g = guard_page();
	s = g - 8; d = g - 16;
	for (i = 0; i < 8; i++) { s[i] = (unsigned char)(i + 1); d[i] = 0; }
	pc_test();
	(void)f_memcpy(d, s, 8);
	for (i = 0; i < 8; i++)
		pc_check(d[i] == s[i], "byte %d was not copied", i);
	(void)f_memcpy(s, d, 8);
	pc_check(f_memcpy(g, g - 8, 0) == g, "n 0 on the boundary was not accepted");
	pc_pass();
}
