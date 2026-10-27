/*
 * wcsnlen() examines no more than maxlen codes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcslen.003
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <wchar.h>

/*
 * Setup: a writable page followed by an inaccessible one (PROT_NONE);
 * returns the address where the inaccessible page begins.
 */
static wchar_t *guard_page(void)
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
	return (wchar_t *)(void *)(p + ps);
}

int main(void)
{
	size_t a, b, c;
	wchar_t *g;
	int i;

	pc_begin("XSH.wcslen.003");
	g = guard_page();
	for (i = 1; i <= 16; i++)
		g[-i] = L'q';	/* no null code before the page */
	pc_test();
	a = wcsnlen(g - 16, 16);	/* a fault here (reading the page) is a FAIL */
	b = wcsnlen(g - 5, 5);
	c = wcsnlen(g, 0);
	pc_check(a == 16, "wcsnlen() of 16 non-null codes with maxlen 16 returned %zu", a);
	pc_check(b == 5, "wcsnlen() of 5 non-null codes with maxlen 5 returned %zu", b);
	pc_check(c == 0, "wcsnlen(s, 0) returned %zu", c);
	pc_pass();
}
