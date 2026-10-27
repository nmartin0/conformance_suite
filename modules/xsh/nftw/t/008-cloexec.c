/*
 * Descriptors opened by nftw() have FD_CLOEXEC set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nftw.008
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

static int seen, bad;

static int watch(const char *p, const struct stat *sb, int t, struct FTW *f)
{
	int fd, fl;

	(void)p; (void)sb; (void)t; (void)f;
	for (fd = 0; fd < NFD; fd++) {
		if (before[fd])
			continue;
		fl = fcntl(fd, F_GETFD);
		if (fl == -1)
			continue;
		seen++;
		if (!(fl & FD_CLOEXEC))
			bad++;
	}
	return 0;
}

int main(void)
{
	int r;

	pc_begin("XSH.nftw.008");
	make_tree();
	pc_test();
	snap(before);
	r = nftw("t", watch, 10, 0);
	pc_check(r == 0, "nftw() returned %d, expected 0", r);
	pc_check(bad == 0, "%d descriptor(s) opened by nftw() lacked FD_CLOEXEC", bad);
	if (seen == 0)
		pc_notinuse("nftw() held no descriptor while calling fn");
	pc_pass();
}
