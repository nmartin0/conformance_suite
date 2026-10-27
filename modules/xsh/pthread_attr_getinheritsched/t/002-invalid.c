/*
 * pthread_attr_setinheritsched() fails for a value that is neither constant.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_attr_getinheritsched.002
 */
#include "pc.h"
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

#if defined(_POSIX_THREAD_PRIORITY_SCHEDULING) && _POSIX_THREAD_PRIORITY_SCHEDULING >= 0
static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

int main(void)
{
	static const int bad[] = { 0x7ffffff0, -0x7ffffff0 };
	pthread_attr_t a;
	unsigned i;
	int r;

	pc_begin("XSH.pthread_attr_getinheritsched.002");
	NEED(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
	for (i = 0; i < 2; i++)
		NEED(bad[i] != PTHREAD_INHERIT_SCHED && bad[i] != PTHREAD_EXPLICIT_SCHED,
		    "%#x is the value of a standard inheritsched here", (unsigned)bad[i]);
	pc_test();
	for (i = 0; i < 2; i++) {
		r = pthread_attr_setinheritsched(&a, bad[i]);
		pc_check(r == ENOTSUP || r == EINVAL, "pthread_attr_setinheritsched(%d) returned %s, expected ENOTSUP or EINVAL",
		    bad[i], rn(r));
	}
	(void)pthread_attr_destroy(&a);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_attr_getinheritsched.002");
	pc_unsupported("_POSIX_THREAD_PRIORITY_SCHEDULING is not defined");
}
#endif
