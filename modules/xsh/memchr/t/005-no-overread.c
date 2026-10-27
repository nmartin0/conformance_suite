/*
 * memchr: no overread.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memchr.005
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
static void *(*volatile f_memchr)(const void *, int, size_t) = memchr;

int main(void)
{
	unsigned char *g, *p;
	pc_begin("XSH.memchr.005");
	g = guard_page();
	p = g - 4;
	p[0] = 'q'; p[1] = 'r'; p[2] = 's'; p[3] = 't';
	pc_test();
	pc_check(f_memchr(p, 'z', 4) == NULL, "an absent byte was reported present");
	pc_check(f_memchr(p, 't', 4) == p + 3, "the last byte was not found");
	pc_check(f_memchr(p, 'q', 8192) == p, "a first-byte match was not found before reading on");
	pc_check(f_memchr(p, 's', 100000) == p + 2, "a match was not found before reading on");
	pc_pass();
}
