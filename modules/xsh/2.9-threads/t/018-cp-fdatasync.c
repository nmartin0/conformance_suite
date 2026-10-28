/*
 * fdatasync(), a cancellation point, acts on a pending cancellation request
 * before it returns.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.018
 *
 * See XSH.2.9-threads.012.
 */
#include "pccp.h"
#include <fcntl.h>

static volatile long sink;
static int ffd;

static void c_fdatasync(void) { sink = fdatasync(ffd); }

static const struct cpcase cases[] = {
	{ "fdatasync", NULL, c_fdatasync, NULL },
};

int main(void)
{
	char bad[256];
	int n;

	pc_begin("XSH.2.9-threads.018");
	guard(120);
	pc_make_file("f", "0123456789");
	ffd = open("f", O_RDWR);
	NEED(ffd != -1, "open failed");
	pc_test();
	n = cp_run(cases, 1, CP_PENDING, bad, sizeof bad);
	pc_check(n == 0, "with a cancellation request pending and cancelability enabled the thread was not canceled in: %s", bad);
	pc_pass();
}
