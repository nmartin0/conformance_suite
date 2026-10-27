/*
 * Without FTW_DEPTH a directory is reported before the objects in it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nftw.004
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

static void need(const char *name, int type, int level)
{
	int i = idx(name);

	pc_check(i >= 0, "%s was not reported exactly once", name);
	pc_check(rec[i].type == type, "%s was reported with type %d, expected %d", name, rec[i].type, type);
	pc_check(rec[i].level == level, "%s was reported at level %d, expected %d", name, rec[i].level, level);
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
	int r;

	pc_begin("XSH.nftw.004");
	make_tree();
	pc_test();
	r = nftw("t", record, 10, 0);
	pc_check(r == 0, "nftw() returned %d, expected 0", r);
	pc_check(nrec == 6, "fn was called %d times, expected 6", nrec);
	need("t", FTW_D, 0);
	need("d", FTW_D, 1);
	need("e", FTW_D, 2);
	need("a", FTW_F, 1);
	need("b", FTW_F, 2);
	need("c", FTW_F, 3);
	pc_check(idx("t") == 0, "the root was not reported first");
	pc_check(idx("d") < idx("b") && idx("d") < idx("e"), "d was not reported before its contents");
	pc_check(idx("e") < idx("c"), "e was not reported before c");
	pc_pass();
}
