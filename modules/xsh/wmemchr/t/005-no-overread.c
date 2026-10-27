/*
 * wmemchr() examines no more than n elements.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wmemchr.005
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
	wchar_t *g, *absent, *present;
	int i;

	pc_begin("XSH.wmemchr.005");
	g = guard_page();
	for (i = 1; i <= 4; i++)
		g[-i] = L'q';
	pc_test();
	absent = wmemchr(g - 4, L'z', 4);	/* a fault here (reading the page) is a FAIL */
	present = wmemchr(g - 4, L'q', 4);
	pc_check(absent == NULL, "an absent code was reported present");
	pc_check(present == g - 4, "a code at the first element was not found there");
	pc_pass();
}
