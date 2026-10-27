/*
 * Each contention scope is set and read back, or refused with ENOTSUP.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_attr_getscope.002
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
	static const int scopes[] = { PTHREAD_SCOPE_SYSTEM, PTHREAD_SCOPE_PROCESS };
	static const char *const names[] = { "PTHREAD_SCOPE_SYSTEM", "PTHREAD_SCOPE_PROCESS" };
	pthread_attr_t a;
	int supported = 0, i, v, r;

	pc_begin("XSH.pthread_attr_getscope.002");
	NEED(pthread_attr_init(&a) == 0, "pthread_attr_init failed");
	pc_test();
	for (i = 0; i < 2; i++) {
		r = pthread_attr_setscope(&a, scopes[i]);
		pc_check(r == 0 || r == ENOTSUP, "pthread_attr_setscope(%s) returned %s, expected 0 or ENOTSUP", names[i], rn(r));
		if (r != 0)
			continue;
		supported++;
		v = -12345;
		r = pthread_attr_getscope(&a, &v);
		pc_check(r == 0, "pthread_attr_getscope() returned %s", rn(r));
		pc_check(v == scopes[i], "after setting %s (%d), getscope stored %d", names[i], scopes[i], v);
	}
	pc_check(supported > 0, "neither contention scope can be set");
	(void)pthread_attr_destroy(&a);
	pc_pass();
}
#else
int main(void)
{
	pc_begin("XSH.pthread_attr_getscope.002");
	pc_unsupported("_POSIX_THREAD_PRIORITY_SCHEDULING is not defined");
}
#endif
