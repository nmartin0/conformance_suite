/*
 * memccpy() without c copies n bytes and returns a null pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.memccpy.002
 * MUTANT    memccpy.not_null
 * MUTANT    memccpy.reads_extra
 * MUTANT    memccpy.copies_extra
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

/*
 * Setup: a writable page followed by an inaccessible one (PROT_NONE);
 * returns the address where the inaccessible page begins.
 */
static char *guard_page(void)
{
	long ps = sysconf(_SC_PAGESIZE);
	char *p;

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

static char dst[32];

/* 1 if p[0..n) are all 'X'. */
static int untouched(const char *p, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++)
		if (p[i] != 'X')
			return 0;
	return 1;
}

int main(void)
{
	char *g, *s;
	void *r;

	pc_begin("XSH.memccpy.002");
	g = guard_page();
	s = g - 12;
	memcpy(s, "0123456789ab", 12);
	pc_test();
	memset(dst, 'X', sizeof dst);
	r = memccpy(dst, s, 'z', 12);	/* a fault here (reading the page) is a FAIL */
	pc_check(memcmp(dst, "0123456789ab", 12) == 0, "memccpy() did not copy the 12 bytes");
	pc_check(untouched(dst + 12, sizeof dst - 12), "memccpy() wrote past n bytes");
	pc_check(r == NULL, "memccpy() without c in the 12 bytes did not return a null pointer");
	memset(dst, 'X', sizeof dst);
	r = memccpy(dst, s, '0', 0);
	pc_check(untouched(dst, sizeof dst), "memccpy() with n 0 wrote to s1");
	pc_check(r == NULL, "memccpy() with n 0 did not return a null pointer");
	pc_pass();
}
