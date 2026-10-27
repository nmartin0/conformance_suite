/*
 * FTW_MOUNT and FTW_XDEV change nothing on a tree on one file system.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nftw.014
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

#define MAXREC 64

struct rec {
	char path[160];
	int type, level, base;
	struct stat sb;
};

static struct rec rec[MAXREC];
static int nrec;

static int record(const char *p, const struct stat *sb, int t, struct FTW *f)
{
	if (nrec < MAXREC) {
		struct rec *r = &rec[nrec];

		strncpy(r->path, p, sizeof r->path - 1);
		r->path[sizeof r->path - 1] = '\0';
		r->type = t;
		r->level = f->level;
		r->base = f->base;
		if (t != FTW_NS)
			r->sb = *sb;
	}
	nrec++;
	return 0;
}

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


int main(void)
{
	static const int flags[] = { 0, FTW_MOUNT, FTW_XDEV, FTW_MOUNT | FTW_XDEV };
	int r, i;

	pc_begin("XSH.nftw.014");
	make_tree();
	pc_make_symlink("a", "t/s");
	pc_test();
	for (i = 0; i < 4; i++) {
		nrec = 0;
		r = nftw("t", record, 10, flags[i]);
		pc_check(r == 0, "nftw() with flags %#x returned %d, expected 0", (unsigned)flags[i], r);
		pc_check(nrec == 7, "nftw() with flags %#x reported %d objects, expected 7", (unsigned)flags[i], nrec);
	}
	pc_pass();
}
