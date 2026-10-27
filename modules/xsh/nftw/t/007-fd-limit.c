/*
 * nftw() with fd_limit 1 completes and keeps at most one descriptor open.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nftw.007
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <ftw.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* The three-level tree used by several tests. */
static void make_tree(void)
{
	pc_make_dir("t");
	pc_make_file("t/a", "hello");
	pc_make_dir("t/d");
	pc_make_file("t/d/b", "x");
	pc_make_dir("t/d/e");
	pc_make_file("t/d/e/c", "yy");
}



#define NFD 256

static unsigned char before[NFD];

static void snap(unsigned char *s)
{
	int fd;

	for (fd = 0; fd < NFD; fd++)
		s[fd] = fcntl(fd, F_GETFD) != -1;
}

static int calls, maxextra;

static int watch(const char *p, const struct stat *sb, int t, struct FTW *f)
{
	int fd, extra = 0;

	(void)p; (void)sb; (void)t; (void)f;
	calls++;
	for (fd = 0; fd < NFD; fd++)
		if (!before[fd] && fcntl(fd, F_GETFD) != -1)
			extra++;
	if (extra > maxextra)
		maxextra = extra;
	return 0;
}

int main(void)
{
	int r;

	pc_begin("XSH.nftw.007");
	make_tree();
	pc_test();
	snap(before);
	r = nftw("t", watch, 1, 0);
	pc_check(r == 0, "nftw() with fd_limit 1 returned %d, expected 0", r);
	pc_check(calls == 6, "fn was called %d times, expected 6", calls);
	pc_check(maxextra <= 1, "nftw() held %d descriptors with fd_limit 1", maxextra);
	pc_pass();
}
