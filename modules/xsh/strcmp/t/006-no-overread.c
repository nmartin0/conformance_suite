/*
 * strcmp: no overread.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strcmp.006
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
static int (*volatile f_strcmp)(const char *, const char *) = strcmp;

int main(void)
{
	unsigned char *g1, *g2, *p, *q;
	pc_begin("XSH.strcmp.006");
	g1 = guard_page();
	g2 = guard_page();
	p = g1 - 4; q = g2 - 4;
	memcpy(p, "abc", 4);
	memcpy(q, "abc", 4);
	pc_test();
	pc_check(f_strcmp((const char *)p, (const char *)q) == 0, "equal strings did not compare equal");
	q[2] = 'd';
	pc_check(f_strcmp((const char *)p, (const char *)q) < 0, "\"abc\" was not less than \"abd\"");
	pc_check(f_strcmp((const char *)q, (const char *)p) > 0, "\"abd\" was not greater than \"abc\"");
	pc_pass();
}
