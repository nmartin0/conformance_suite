/*
 * Without FTW_PHYS and FTW_DEPTH nothing is reported below a link to an ancestor.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nftw.012
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

/* The object's own name: the part of the pathname at base. */
static const char *leaf(const struct rec *r)
{
	if (r->base < 0 || (size_t)r->base > strlen(r->path))
		return "";
	return r->path + r->base;
}

/* Index of the one record named name; -1 if none, -2 if several. */
static int idx(const char *name)
{
	int i, found = -1;

	for (i = 0; i < nrec && i < MAXREC; i++)
		if (strcmp(leaf(&rec[i]), name) == 0) {
			if (found != -1)
				return -2;
			found = i;
		}
	return found;
}


/* Stops a runaway walk: more than 40 calls is far beyond the tree. */
static int guard(const char *p, const struct stat *sb, int t, struct FTW *f)
{
	record(p, sb, t, f);
	return nrec > 40 ? 99 : 0;
}

int main(void)
{
	int r, i;

	pc_begin("XSH.nftw.012");
	pc_make_dir("t");
	pc_make_dir("t/d");
	pc_make_file("t/d/b", "x");
	pc_make_symlink("..", "t/d/up");
	pc_test();
	r = nftw("t", guard, 10, 0);
	pc_check(r == 0, "nftw() returned %d, expected 0 (99 means the walk did not end)", r);
	for (i = 0; i < nrec; i++)
		pc_check(strstr(rec[i].path, "/up/") == NULL, "%s was reported below the link to an ancestor", rec[i].path);
	pc_check(idx("t") >= 0 && idx("d") >= 0 && idx("b") >= 0, "t, d and b were not each reported once");
	pc_pass();
}
