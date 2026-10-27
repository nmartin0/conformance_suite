/*
 * wcsncmp() reads no more than n codes of each array.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.wcsncmp.004
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

static wchar_t other[] = { L'q', L'q', L'q', L'Z', 0 };

int main(void)
{
	wchar_t *g;
	int r[4];
	int i;

	pc_begin("XSH.wcsncmp.004");
	g = guard_page();
	for (i = 1; i <= 3; i++)
		g[-i] = L'q';	/* three codes, no null code, ending at the page */
	pc_test();
	r[0] = wcsncmp(g - 3, other, 3);	/* a fault here (reading the page) is a FAIL */
	r[1] = wcsncmp(other, g - 3, 3);
	r[2] = wcsncmp(g - 2, other, 2);
	r[3] = wcsncmp(other, g - 2, 2);
	for (i = 0; i < 4; i++)
		pc_check(r[i] == 0, "call %d on equal codes with n bounding the array returned %d", i, r[i]);
	pc_pass();
}
