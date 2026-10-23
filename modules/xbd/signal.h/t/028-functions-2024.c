/*
 * sig2str() and str2sig() are declared.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.signal.h.028
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
	pc_begin("XBD.signal.h.028");
	pc_test();
	pc_check(IS_T(&(sig2str), int (*)(int, char *)), "sig2str() is not declared as int (*)(int, char *)");
	pc_check(!IS_T(&(sig2str), int (*)(int, char *, int)), "sig2str() is declared without a prototype");
	pc_check(IS_T(&(str2sig), int (*)(const char *, int *)), "str2sig() is not declared as int (*)(const char *, int *)");
	pc_check(!IS_T(&(str2sig), int (*)(const char *, int *, int)), "str2sig() is declared without a prototype");
	pc_pass();
}
