/*
 * siginfo_t has the required members with their types.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.signal.h.021
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
	pc_begin("XBD.signal.h.021");
	pc_test();
	{
		siginfo_t si;

		pc_check(IS_T(si.si_signo, int) && IS_T(si.si_code, int) && IS_T(si.si_status, int),
		    "si_signo, si_code or si_status is not an int");
		pc_check(IS_T(si.si_pid, pid_t), "si_pid is not a pid_t");
		pc_check(IS_T(si.si_uid, uid_t), "si_uid is not a uid_t");
		pc_check(IS_T(si.si_addr, void *), "si_addr is not a void *");
		pc_check(IS_T(si.si_value, union sigval), "si_value is not a union sigval");
	}
	pc_pass();
}
