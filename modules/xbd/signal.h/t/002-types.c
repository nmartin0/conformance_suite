/*
 * <signal.h> defines sig_atomic_t, sigset_t and the types it borrows.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.signal.h.002
 *
 * Only <signal.h> is included (pc.h brings in <stddef.h> alone), so a
 * type or name this test uses that the header does not provide stops
 * the build, and the test is UNINITIATED.
 */
#include <signal.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

/* Fails unless the n values in v are all different. */
static void distinct(const char *what, const long *v, const char *const *name, int n)
{
	int i, j;

	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++)
			pc_check(v[i] != v[j], "%s: %s and %s are both %ld", what, name[i], name[j], v[i]);
}

int main(void)
{
	pc_begin("XBD.signal.h.002");
	pc_test();
	{
		sig_atomic_t a = 3;
		sigset_t s, s2;
		pid_t p = 1;
		uid_t u = 0;
		pthread_t th;
		pthread_attr_t *at = NULL;
		struct timespec ts;

		ts.tv_sec = 1;
		ts.tv_nsec = 2;
		pc_check(sigemptyset(&s) == 0, "sigemptyset failed");
		s2 = s;	/* an object type that can be assigned */
		(void)s2;
		(void)th;
		(void)at;
		pc_check(a / 2 == 1 && (sig_atomic_t)2.5 == 2, "sig_atomic_t does not behave as an integer type");
		pc_check(p == 1 && u == 0 && ts.tv_sec + ts.tv_nsec == 3, "pid_t, uid_t or struct timespec misbehaves");
	}
	pc_pass();
}
