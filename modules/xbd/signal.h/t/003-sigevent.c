/*
 * struct sigevent has the required members with their types.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.signal.h.003
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
	pc_begin("XBD.signal.h.003");
	pc_test();
	{
		struct sigevent ev;

		pc_check(IS_T(ev.sigev_notify, int), "sigev_notify is not an int");
		pc_check(IS_T(ev.sigev_signo, int), "sigev_signo is not an int");
		pc_check(IS_T(ev.sigev_value, union sigval), "sigev_value is not a union sigval");
		pc_check(IS_T(ev.sigev_notify_function, void (*)(union sigval)), "sigev_notify_function is not void (*)(union sigval)");
		pc_check(IS_T(ev.sigev_notify_attributes, pthread_attr_t *), "sigev_notify_attributes is not pthread_attr_t *");
	}
	pc_pass();
}
