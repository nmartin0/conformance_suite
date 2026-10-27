/*
 * pthread_condattr_init() gives the process-shared attribute its default.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_condattr_init.003
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#if defined(_POSIX_THREAD_PROCESS_SHARED) && _POSIX_THREAD_PROCESS_SHARED >= 0
static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

static void check_default(pthread_condattr_t *a, const char *when)
{
	int p = -12345, r;

	r = pthread_condattr_getpshared(a, &p);
	pc_check(r == 0, "%s: pthread_condattr_getpshared() returned %s", when, rn(r));
	pc_check(p == PTHREAD_PROCESS_PRIVATE, "%s: process-shared is %d, expected PTHREAD_PROCESS_PRIVATE (%d)", when, p,
	    (int)PTHREAD_PROCESS_PRIVATE);
}

int main(void)
{
	static const unsigned char fill[] = { 0x00, 0xff, 0xa5 };
	pthread_condattr_t a;
	unsigned i;
	int r;

	pc_begin("XSH.pthread_condattr_init.003");
	pc_test();
	for (i = 0; i < sizeof fill; i++) {
		memset(&a, fill[i], sizeof a);
		r = pthread_condattr_init(&a);
		pc_check(r == 0, "pthread_condattr_init() on storage filled with %#x returned %s", fill[i], rn(r));
		check_default(&a, "after init on filled storage");
		(void)pthread_condattr_destroy(&a);
	}
	r = pthread_condattr_init(&a);
	pc_check(r == 0, "pthread_condattr_init() returned %s", rn(r));
	r = pthread_condattr_setpshared(&a, PTHREAD_PROCESS_SHARED);
	if (r != 0)
		pc_unresolved("pthread_condattr_setpshared(SHARED) returned %s", rn(r));
	r = pthread_condattr_destroy(&a);
	pc_check(r == 0, "pthread_condattr_destroy() returned %s", rn(r));
	r = pthread_condattr_init(&a);
	pc_check(r == 0, "pthread_condattr_init() after the destroy returned %s", rn(r));
	check_default(&a, "after destroy and init of a changed object");
	(void)pthread_condattr_destroy(&a);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_condattr_init.003");
	pc_unsupported("_POSIX_THREAD_PROCESS_SHARED is not defined");
}
#endif
