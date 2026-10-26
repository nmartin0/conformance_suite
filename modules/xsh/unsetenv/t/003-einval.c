/*
 * unsetenv() fails with EINVAL for an empty name or one containing '=',
 * and leaves the environment unchanged.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.unsetenv.003
 * MUTANT    unsetenv.accepts_empty
 * MUTANT    unsetenv.cuts_at_equals
 * MUTANT    unsetenv.removes_then_fails
 * VARIANT   unsetenv.swap_last
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

/* The environment's strings, copied and sorted: compared as a set. */
struct snap {
	size_t n;
	char **s;
};

/* qsort() and bsearch() order: by string. */
static int by_string(const void *a, const void *b)
{
	return strcmp(*(char *const *)a, *(char *const *)b);
}

/* A snapshot of the environment now. */
static struct snap snapshot(void)
{
	struct snap t = { 0, NULL };
	size_t i;

	while (environ[t.n] != NULL)
		t.n++;
	t.s = malloc((t.n + 1) * sizeof *t.s);
	if (t.s == NULL)
		pc_unresolved("out of memory");
	for (i = 0; i < t.n; i++)
		if ((t.s[i] = strdup(environ[i])) == NULL)
			pc_unresolved("out of memory");
	qsort(t.s, t.n, sizeof *t.s, by_string);
	return t;
}

/* 1 if a and b hold the same strings. */
static int same_set(struct snap a, struct snap b)
{
	size_t i;

	if (a.n != b.n)
		return 0;
	for (i = 0; i < a.n; i++)
		if (strcmp(a.s[i], b.s[i]) != 0)
			return 0;
	return 1;
}

/* The number of strings in environ equal to s. */
static int count_string(const char *s)
{
	size_t i;
	int c = 0;

	for (i = 0; environ[i] != NULL; i++)
		if (strcmp(environ[i], s) == 0)
			c++;
	return c;
}

int main(void)
{
	static const char *const bad[] = { "", "PCX_U=x" };
	struct snap before, after;
	size_t i;
	int r, e;

	pc_begin("XSH.unsetenv.003");
	pc_check(setenv("PCX_U", "u", 1) == 0, "setenv failed: %s", pc_errname(errno));
	pc_test();
	for (i = 0; i < sizeof bad / sizeof bad[0]; i++) {
		before = snapshot();
		errno = 0;
		r = unsetenv(bad[i]);
		e = errno;
		after = snapshot();
		pc_check(r == -1, "unsetenv(\"%s\") returned %d, expected -1", bad[i], r);
		pc_check(e == EINVAL, "unsetenv(\"%s\") set errno %s, expected EINVAL", bad[i], pc_errname(e));
		pc_check(same_set(before, after), "the failed unsetenv(\"%s\") changed the environment", bad[i]);
	}
	pc_check(count_string("PCX_U=u") == 1, "PCX_U=u is no longer in environ");
	pc_pass();
}
