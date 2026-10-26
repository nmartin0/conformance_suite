/*
 * stpcpy() copies the string and returns the end of the copy.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.stpcpy.001
 * MUTANT    stpcpy.returns_s1
 * MUTANT    stpcpy.past_nul
 * MUTANT    stpcpy.no_nul
 * MUTANT    stpcpy.writes_past
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

static char src[3][301], dst[512];

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
	static char hello[] = "hello";
	size_t len[3] = { 0, 1, 300 }, i;
	char *g, *r;

	pc_begin("XSH.stpcpy.001");
	src[1][0] = 'a';
	for (i = 0; i < 300; i++)
		src[2][i] = (char)('A' + i % 26);
	g = guard_page();
	pc_test();
	for (i = 0; i < 3; i++) {
		memset(dst, 'X', sizeof dst);
		r = stpcpy(dst, src[i]);
		pc_check(memcmp(dst, src[i], len[i] + 1) == 0, "stpcpy() of a %zu-byte string did not copy it with its null byte", len[i]);
		pc_check(untouched(dst + len[i] + 1, sizeof dst - len[i] - 1), "stpcpy() of a %zu-byte string changed bytes after the copy", len[i]);
		pc_check(r == dst + len[i], "stpcpy() of a %zu-byte string returned s1%+td, expected s1+strlen(s2)", len[i], r - dst);
	}
	r = stpcpy(g - 6, hello);	/* a fault here (writing the page) is a FAIL */
	pc_check(memcmp(g - 6, "hello", 6) == 0, "stpcpy() into a 6-byte array did not copy \"hello\"");
	pc_check(r == g - 1, "stpcpy() into a 6-byte array returned s1%+td, expected s1+strlen(s2)", r - (g - 6));
	pc_pass();
}
