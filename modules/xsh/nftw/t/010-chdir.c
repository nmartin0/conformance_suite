/*
 * With FTW_CHDIR a regular file is found by its own name in the current directory.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nftw.010
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


static int files, bad;

static int check(const char *p, const struct stat *sb, int t, struct FTW *f)
{
	struct stat s;

	if (t == FTW_F) {
		files++;
		if (f->base < 0 || (size_t)f->base > strlen(p) || lstat(p + f->base, &s) != 0 ||
		    s.st_ino != sb->st_ino || s.st_dev != sb->st_dev)
			bad++;
	}
	return 0;
}

int main(void)
{
	int r, home;

	pc_begin("XSH.nftw.010");
	make_tree();
	home = open(".", O_RDONLY);
	pc_check(home >= 0, "cannot open the working directory");
	pc_test();
	r = nftw("t", check, 10, FTW_CHDIR);
	if (fchdir(home) != 0)
		pc_unresolved("cannot return to the working directory");
	pc_check(r == 0, "nftw() returned %d, expected 0", r);
	pc_check(files == 3, "fn was called for %d files, expected 3", files);
	pc_check(bad == 0, "%d file(s) were not found by name in the current directory", bad);
	pc_pass();
}
