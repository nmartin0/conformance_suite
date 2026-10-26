/*
 * A new mutex attributes object has type PTHREAD_MUTEX_DEFAULT.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutexattr_settype.001
 * MUTANT    pthread_mutexattr_settype.default_errorcheck
 * MUTANT    pthread_mutexattr_settype.gettype_einval
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	pthread_mutexattr_t a;
	int t = -12345, r;

	pc_begin("XSH.pthread_mutexattr_settype.001");
	NEED(pthread_mutexattr_init(&a) == 0, "pthread_mutexattr_init failed");
	pc_test();
	r = pthread_mutexattr_gettype(&a, &t);
	pc_check(r == 0, "pthread_mutexattr_gettype() returned %s", rn(r));
	pc_check(t == PTHREAD_MUTEX_DEFAULT, "a new attributes object has type %d, expected PTHREAD_MUTEX_DEFAULT (%d)",
	    t, (int)PTHREAD_MUTEX_DEFAULT);
	pc_pass();
}
