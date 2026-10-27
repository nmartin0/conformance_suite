/*
 * Without FTW_CHDIR the current directory is not changed.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nftw.011
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


static struct stat home;
static int calls, moved;

static int check(const char *p, const struct stat *sb, int t, struct FTW *f)
{
	struct stat s;

	(void)p; (void)sb; (void)t; (void)f;
	calls++;
	if (stat(".", &s) != 0 || s.st_ino != home.st_ino || s.st_dev != home.st_dev)
		moved++;
	return 0;
}

int main(void)
{
	int r;

	pc_begin("XSH.nftw.011");
	make_tree();
	pc_check(stat(".", &home) == 0, "cannot stat the working directory");
	pc_test();
	r = nftw("t", check, 10, 0);
	pc_check(r == 0, "nftw() returned %d, expected 0", r);
	pc_check(calls == 6 && moved == 0, "with flags 0: %d calls, the directory changed in %d", calls, moved);
	calls = moved = 0;
	r = nftw("t", check, 10, FTW_DEPTH | FTW_PHYS);
	pc_check(r == 0, "nftw() returned %d, expected 0", r);
	pc_check(calls == 6 && moved == 0, "with FTW_DEPTH|FTW_PHYS: %d calls, the directory changed in %d", calls, moved);
	pc_pass();
}
