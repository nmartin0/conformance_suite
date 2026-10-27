/*
 * nftw() closes its descriptors before it returns.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nftw.009
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

static unsigned char after[NFD];
static int calls, stop_at;

static int stopper(const char *p, const struct stat *sb, int t, struct FTW *f)
{
	(void)p; (void)sb; (void)t; (void)f;
	calls++;
	return calls == stop_at;
}

int main(void)
{
	int r, fd;

	pc_begin("XSH.nftw.009");
	make_tree();
	pc_test();
	snap(before);
	stop_at = 0;
	r = nftw("t", stopper, 10, 0);
	pc_check(r == 0, "nftw() returned %d, expected 0", r);
	snap(after);
	for (fd = 0; fd < NFD; fd++)
		pc_check(before[fd] == after[fd], "descriptor %d is %s after a complete walk", fd, after[fd] ? "left open" : "closed");
	calls = 0; stop_at = 5;
	r = nftw("t", stopper, 10, 0);
	pc_check(r == 1, "nftw() returned %d, expected 1", r);
	snap(after);
	for (fd = 0; fd < NFD; fd++)
		pc_check(before[fd] == after[fd], "descriptor %d is %s after a walk ended by fn", fd, after[fd] ? "left open" : "closed");
	pc_pass();
}
