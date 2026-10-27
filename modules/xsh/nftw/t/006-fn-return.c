/*
 * A non-zero return from fn ends the walk and is returned by nftw().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nftw.006
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


static int calls, stop_at, stop_val;

static int stopper(const char *p, const struct stat *sb, int t, struct FTW *f)
{
	(void)p; (void)sb; (void)t; (void)f;
	calls++;
	return calls == stop_at ? stop_val : 0;
}

int main(void)
{
	int r;

	pc_begin("XSH.nftw.006");
	make_tree();
	pc_test();
	stop_at = 1; stop_val = 1;
	r = nftw("t", stopper, 10, 0);
	pc_check(r == 1, "nftw() returned %d, expected the value 1 returned by fn", r);
	pc_check(calls == 1, "fn was called %d times after returning 1, expected 1", calls);
	calls = 0; stop_at = 3; stop_val = 42;
	r = nftw("t", stopper, 10, 0);
	pc_check(r == 42, "nftw() returned %d, expected the value 42 returned by fn", r);
	pc_check(calls == 3, "fn was called %d times, expected 3", calls);
	pc_pass();
}
