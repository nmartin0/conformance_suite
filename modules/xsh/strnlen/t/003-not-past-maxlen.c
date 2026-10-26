/*
 * strnlen() examines no more than maxlen bytes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strnlen.003
 * MUTANT    strnlen.reads_extra
 * MUTANT    strnlen.zero_reads
 * MUTANT    strnlen.uses_strlen
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

int main(void)
{
	size_t a, b, c;
	char *g;

	pc_begin("XSH.strnlen.003");
	g = guard_page();
	memset(g - 16, 'q', 16);	/* no null byte before the page */
	pc_test();
	a = strnlen(g - 16, 16);	/* a fault here (reading the page) is a FAIL */
	b = strnlen(g - 5, 5);
	c = strnlen(g, 0);
	pc_check(a == 16, "strnlen() of 16 non-null bytes with maxlen 16 returned %zu", a);
	pc_check(b == 5, "strnlen() of 5 non-null bytes with maxlen 5 returned %zu", b);
	pc_check(c == 0, "strnlen(s, 0) returned %zu", c);
	pc_pass();
}
