/*
 * pthread_mutexattr_gettype() reports the type pthread_mutexattr_settype() set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutexattr_settype.002
 * MUTANT    pthread_mutexattr_settype.settype_ignored
 * MUTANT    pthread_mutexattr_settype.settype_first_only
 * MUTANT    pthread_mutexattr_settype.gettype_constant
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
	static const int types[] = {
		PTHREAD_MUTEX_NORMAL, PTHREAD_MUTEX_ERRORCHECK, PTHREAD_MUTEX_RECURSIVE, PTHREAD_MUTEX_DEFAULT,
		PTHREAD_MUTEX_RECURSIVE, PTHREAD_MUTEX_ERRORCHECK, PTHREAD_MUTEX_NORMAL
	};
	static const char *const names[] = {
		"NORMAL", "ERRORCHECK", "RECURSIVE", "DEFAULT", "RECURSIVE", "ERRORCHECK", "NORMAL"
	};
	pthread_mutexattr_t a;
	unsigned i;
	int t, r;

	pc_begin("XSH.pthread_mutexattr_settype.002");
	NEED(pthread_mutexattr_init(&a) == 0, "pthread_mutexattr_init failed");
	pc_test();
	for (i = 0; i < sizeof types / sizeof types[0]; i++) {
		r = pthread_mutexattr_settype(&a, types[i]);
		pc_check(r == 0, "pthread_mutexattr_settype(PTHREAD_MUTEX_%s) returned %s", names[i], rn(r));
		t = -12345;
		r = pthread_mutexattr_gettype(&a, &t);
		pc_check(r == 0, "pthread_mutexattr_gettype() returned %s", rn(r));
		pc_check(t == types[i], "after pthread_mutexattr_settype(PTHREAD_MUTEX_%s) (%d), gettype stored %d",
		    names[i], types[i], t);
	}
	pc_pass();
}
