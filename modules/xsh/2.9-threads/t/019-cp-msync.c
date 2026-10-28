/*
 * msync(), a cancellation point, acts on a pending cancellation request
 * before it returns.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.019
 *
 * See XSH.2.9-threads.012. The call is made on a shared mapping of a file.
 */
#include "pccp.h"
#include <fcntl.h>
#include <sys/mman.h>

static volatile long sink;
static void *map;
static size_t len;

static void c_msync(void) { sink = msync(map, len, MS_ASYNC); }

static const struct cpcase cases[] = {
	{ "msync", NULL, c_msync, NULL },
};

int main(void)
{
	char bad[256];
	int n, fd;

	pc_begin("XSH.2.9-threads.019");
	guard(120);
	len = (size_t)sysconf(_SC_PAGESIZE);
	fd = open("f", O_RDWR | O_CREAT | O_EXCL, 0600);
	NEED(fd != -1, "open failed");
	NEED(ftruncate(fd, (off_t)len) == 0, "ftruncate failed");
	map = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	NEED(map != MAP_FAILED, "mmap failed");
	pc_test();
	n = cp_run(cases, 1, CP_PENDING, bad, sizeof bad);
	pc_check(n == 0, "with a cancellation request pending and cancelability enabled the thread was not canceled in: %s", bad);
	pc_pass();
}
