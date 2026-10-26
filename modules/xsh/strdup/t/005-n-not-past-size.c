/*
 * strndup() reads no more than size bytes of an unterminated array.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strdup.005
 * MUTANT    strdup.n_uses_strlen
 * MUTANT    strdup.n_reads_one_more
 * VARIANT   strdup.alloc_size
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
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
	char *g, *a, *p;

	pc_begin("XSH.strdup.005");
	g = guard_page();
	a = g - 16;
	memcpy(a, "0123456789abcdef", 16);	/* no null byte before the page */
	pc_test();
	p = strndup(a, 16);	/* a fault here (reading the page) is a FAIL */
	pc_check(p != NULL, "strndup(a, 16) returned a null pointer (%s)", pc_errname(errno));
	pc_check(memcmp(p, "0123456789abcdef", 17) == 0, "strndup(a, 16) gave \"%.20s\"", p);
	free(p);
	p = strndup(g, 0);
	pc_check(p != NULL, "strndup(page, 0) returned a null pointer (%s)", pc_errname(errno));
	pc_check(p[0] == '\0', "strndup(page, 0) gave a non-empty string");
	free(p);
	pc_pass();
}
