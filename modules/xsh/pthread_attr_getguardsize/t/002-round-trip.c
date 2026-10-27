/*
 * pthread_attr_getguardsize() reports the size that was set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_attr_getguardsize.002
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	pthread_attr_t a;
	long pg = sysconf(_SC_PAGESIZE);
	size_t want[2], g;
	int i, r;

	pc_begin("XSH.pthread_attr_getguardsize.002");
	NEED(pg > 0, "sysconf(_SC_PAGESIZE) failed");
	NEED(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
	want[0] = (size_t)pg;
	want[1] = (size_t)pg + 1;
	pc_test();
	r = pthread_attr_setguardsize(&a, want[0]);
	if (r == EINVAL)
		pc_unresolved("pthread_attr_setguardsize() refused one page");
	pc_check(r == 0, "pthread_attr_setguardsize(%zu) returned %s", want[0], rn(r));
	for (i = 0; i < 2; i++) {
		if (i == 1) {
			r = pthread_attr_setguardsize(&a, want[1]);
			if (r == EINVAL)
				break;
			pc_check(r == 0, "pthread_attr_setguardsize(%zu) returned %s", want[1], rn(r));
		}
		g = 0;
		r = pthread_attr_getguardsize(&a, &g);
		pc_check(r == 0, "pthread_attr_getguardsize() returned %s", rn(r));
		pc_check(g == want[i], "after setguardsize(%zu), getguardsize stored %zu", want[i], g);
	}
	(void)pthread_attr_destroy(&a);
	pc_pass();
}
