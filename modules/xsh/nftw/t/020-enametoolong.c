/*
 * nftw() of a component longer than {NAME_MAX} fails with ENAMETOOLONG.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nftw.020
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <ftw.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
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
	long max;
	char *name;

	pc_begin("XSH.nftw.020");
	errno = 0;
	max = pathconf(".", _PC_NAME_MAX);
	if (max == -1 && errno == 0)
		pc_notinuse("{NAME_MAX} is unbounded for this directory");
	pc_check(max > 0, "pathconf(_PC_NAME_MAX) failed");
	name = malloc((size_t)max + 2);
	pc_check(name != NULL, "malloc failed");
	memset(name, 'a', (size_t)max + 1);
	name[max + 1] = '\0';
	errno = 0;
	if (pathconf(".", _PC_NO_TRUNC) == 0)
		pc_notinuse("over-long names are truncated here (_PC_NO_TRUNC reports 0 for this directory)");
	pc_test();
	pc_expect_err(nftw(name, counter, 10, 0), ENAMETOOLONG, "nftw(<NAME_MAX+1 bytes>)");
	pc_check(calls == 0, "fn was called after a failed nftw()");
	pc_pass();
}
