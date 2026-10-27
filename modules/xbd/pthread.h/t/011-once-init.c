/*
 * A static pthread_once_t initialised with PTHREAD_ONCE_INIT makes two
 * pthread_once() calls run the routine exactly once.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.pthread.h.011
 */
#include "pc.h"
#include <pthread.h>

static pthread_once_t once = PTHREAD_ONCE_INIT;
static int calls;

static void routine(void)
{
	calls++;
}

int main(void)
{
	int e;

	pc_begin("XBD.pthread.h.011");
	pc_test();
	e = pthread_once(&once, routine);
	pc_check(e == 0, "pthread_once failed (%s)", pc_errname(e));
	e = pthread_once(&once, routine);
	pc_check(e == 0, "the second pthread_once failed (%s)", pc_errname(e));
	pc_check(calls == 1, "the routine ran %d times through a PTHREAD_ONCE_INIT control", calls);
	pc_pass();
}
