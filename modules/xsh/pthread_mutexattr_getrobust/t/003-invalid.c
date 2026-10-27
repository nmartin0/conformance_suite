/*
 * pthread_mutexattr_setrobust() rejects an invalid value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutexattr_getrobust.003
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

int main(void)
{
	static const int bad[] = { 0x7ffffff0, -0x7ffffff0 };
	pthread_mutexattr_t a;
	unsigned i;
	int r;

	pc_begin("XSH.pthread_mutexattr_getrobust.003");
	NEED(pthread_mutexattr_init(&a) == 0, "pthread_mutexattr_init failed");
	for (i = 0; i < 2; i++)
		NEED(bad[i] != PTHREAD_MUTEX_STALLED && bad[i] != PTHREAD_MUTEX_ROBUST,
		    "%#x is the value of a standard robust attribute here", (unsigned)bad[i]);
	pc_test();
	for (i = 0; i < 2; i++) {
		r = pthread_mutexattr_setrobust(&a, bad[i]);
		pc_check(r == EINVAL, "pthread_mutexattr_setrobust(%d) returned %s, expected EINVAL", bad[i],
		    r == 0 ? "0" : pc_errname(r));
	}
	(void)pthread_mutexattr_destroy(&a);
	pc_pass();
}
