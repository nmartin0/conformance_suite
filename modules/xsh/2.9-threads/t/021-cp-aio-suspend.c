/*
 * aio_suspend(), a cancellation point, acts on a pending cancellation
 * request before it returns.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.021
 *
 * See XSH.2.9-threads.012. The list given to aio_suspend() has one null
 * entry (ignored by the call) and a limit 30 s ahead.
 */
#include "pccp.h"
#include <aio.h>

static volatile long sink;

static void c_aio_suspend(void)
{
	const struct aiocb *list[1] = { NULL };
	struct timespec ts = { 30, 0 };

	sink = aio_suspend(list, 1, &ts);
}

static const struct cpcase cases[] = {
	{ "aio_suspend", NULL, c_aio_suspend, NULL },
};

int main(void)
{
	char bad[256];
	int n;

	pc_begin("XSH.2.9-threads.021");
	guard(120);
	pc_test();
	n = cp_run(cases, 1, CP_PENDING, bad, sizeof bad);
	pc_check(n == 0, "with a cancellation request pending and cancelability enabled the thread was not canceled in: %s", bad);
	pc_pass();
}
