/*
 * setenv() fails with EINVAL for an empty name or one containing '=',
 * and leaves the environment unchanged.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setenv.005
 * MUTANT    setenv.accepts_empty
 * MUTANT    setenv.cuts_at_equals
 * MUTANT    setenv.adds_then_fails
 * VARIANT   setenv.front
 * VARIANT   setenv.fresh_array
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

/* The number of strings in environ whose name is name. */
static int count_named(const char *name)
{
	size_t n = strlen(name), i;
	int c = 0;

	for (i = 0; environ[i] != NULL; i++)
		if (strncmp(environ[i], name, n) == 0 && environ[i][n] == '=')
			c++;
	return c;
}

int main(void)
{
	static const char *const bad[] = { "", "PCX_E=x", "PCX_E=" };
	struct snap before, after;
	size_t i;
	int r, e;

	pc_begin("XSH.setenv.005");
	(void)unsetenv("PCX_E");
	pc_test();
	for (i = 0; i < sizeof bad / sizeof bad[0]; i++) {
		before = snapshot();
		errno = 0;
		r = setenv(bad[i], "v", 1);
		e = errno;
		after = snapshot();
		pc_check(r == -1, "setenv(\"%s\", \"v\", 1) returned %d, expected -1", bad[i], r);
		pc_check(e == EINVAL, "setenv(\"%s\", \"v\", 1) set errno %s, expected EINVAL", bad[i], pc_errname(e));
		pc_check(same_set(before, after), "the failed setenv(\"%s\", ...) changed the environment", bad[i]);
	}
	pc_check(count_named("PCX_E") == 0 && getenv("PCX_E") == NULL, "a variable PCX_E appeared");
	pc_pass();
}
