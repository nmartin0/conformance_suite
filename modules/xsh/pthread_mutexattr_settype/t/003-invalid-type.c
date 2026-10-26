/*
 * pthread_mutexattr_settype() with an invalid type fails with EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_mutexattr_settype.003
 * MUTANT    pthread_mutexattr_settype.settype_no_check
 * MUTANT    pthread_mutexattr_settype.settype_enotsup
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
	static const int bad[] = { 0x7ffffff0, -0x7ffffff0 };
	pthread_mutexattr_t a;
	unsigned i;
	int r;

	pc_begin("XSH.pthread_mutexattr_settype.003");
	NEED(pthread_mutexattr_init(&a) == 0, "pthread_mutexattr_init failed");
	for (i = 0; i < 2; i++)
		NEED(bad[i] != PTHREAD_MUTEX_NORMAL && bad[i] != PTHREAD_MUTEX_ERRORCHECK &&
		    bad[i] != PTHREAD_MUTEX_RECURSIVE && bad[i] != PTHREAD_MUTEX_DEFAULT,
		    "%#x is the value of a standard mutex type here", (unsigned)bad[i]);
	pc_test();
	for (i = 0; i < 2; i++) {
		r = pthread_mutexattr_settype(&a, bad[i]);
		pc_check(r == EINVAL, "pthread_mutexattr_settype(%d) returned %s, expected EINVAL", bad[i], rn(r));
	}
	pc_pass();
}
