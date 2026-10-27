/*
 * Without FTW_PHYS a symbolic link to a directory is followed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nftw.003
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


int main(void)
{
	int r, i, nb = 0;

	pc_begin("XSH.nftw.003");
	pc_make_dir("t");
	pc_make_dir("t/d");
	pc_make_file("t/d/b", "x");
	pc_make_symlink("d", "t/sd");
	pc_test();
	r = nftw("t", record, 10, 0);
	pc_check(r == 0, "nftw() returned %d, expected 0", r);
	pc_check(nrec == 5, "fn was called %d times, expected 5", nrec);
	need("t", FTW_D, 0);
	need("d", FTW_D, 1);
	need("sd", FTW_D, 1);
	pc_check(S_ISDIR(rec[idx("sd")].sb.st_mode), "the stat buffer for sd is not that of a directory");
	for (i = 0; i < nrec; i++)
		if (strcmp(leaf(&rec[i]), "b") == 0) {
			nb++;
			pc_check(rec[i].type == FTW_F && rec[i].level == 2, "b was reported with type %d at level %d", rec[i].type, rec[i].level);
		}
	pc_check(nb == 2, "b was reported %d times, expected 2 (under d and under sd)", nb);
	pc_pass();
}
