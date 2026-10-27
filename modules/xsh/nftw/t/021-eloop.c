/*
 * nftw() of a path through a symbolic link loop fails with ELOOP.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nftw.021
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



static int calls;

static int counter(const char *p, const struct stat *sb, int t, struct FTW *f)
{
	(void)p; (void)sb; (void)t; (void)f;
	calls++;
	return 0;
}

int main(void)
{
	pc_begin("XSH.nftw.021");
	pc_make_symlink("l", "l");
	pc_test();
	pc_expect_err(nftw("l/x", counter, 10, 0), ELOOP, "nftw(l/x)");
	pc_check(calls == 0, "fn was called after a failed nftw()");
	pc_pass();
}
