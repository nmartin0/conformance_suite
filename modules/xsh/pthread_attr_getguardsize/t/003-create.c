/*
 * Threads can be created with a guard size of 0 and of one page.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_attr_getguardsize.003
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static int token;

static void *routine(void *arg)
{
	return arg;
}

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	pthread_attr_t a;
	pthread_t t;
	long pg = sysconf(_SC_PAGESIZE);
	size_t sizes[2];
	void *out;
	int i, r;

	pc_begin("XSH.pthread_attr_getguardsize.003");
	NEED(pg > 0, "sysconf(_SC_PAGESIZE) failed");
	NEED(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
	sizes[0] = 0;
	sizes[1] = (size_t)pg;
	pc_test();
	for (i = 0; i < 2; i++) {
		r = pthread_attr_setguardsize(&a, sizes[i]);
		if (r == EINVAL && i == 1)
			pc_unresolved("pthread_attr_setguardsize() refused one page");
		pc_check(r == 0, "pthread_attr_setguardsize(%zu) returned %s", sizes[i], rn(r));
		r = pthread_create(&t, &a, routine, &token);
		if (r == EAGAIN)
			pc_unresolved("pthread_create() failed with EAGAIN");
		pc_check(r == 0, "pthread_create() with guard size %zu returned %s", sizes[i], rn(r));
		out = NULL;
		r = pthread_join(t, &out);
		pc_check(r == 0, "pthread_join() returned %s", rn(r));
		pc_check(out == &token, "the thread created with guard size %zu returned %p, expected %p", sizes[i], out,
		    (void *)&token);
	}
	(void)pthread_attr_destroy(&a);
	pc_pass();
}
