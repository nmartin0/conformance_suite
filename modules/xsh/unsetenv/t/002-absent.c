/*
 * unsetenv() of a name not in the environment succeeds and changes
 * nothing.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.unsetenv.002
 * MUTANT    unsetenv.absent_fails
 * MUTANT    unsetenv.absent_drops_last
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
	struct snap before, after;
	int r;

	pc_begin("XSH.unsetenv.002");
	(void)unsetenv("PCX_ABSENT");
	pc_check(setenv("PCX_V", "v", 1) == 0, "setenv failed: %s", pc_errname(errno));
	pc_check(count_named("PCX_ABSENT") == 0, "PCX_ABSENT is in the environment");
	pc_test();
	before = snapshot();
	errno = 0;
	r = unsetenv("PCX_ABSENT");
	after = snapshot();
	pc_check(r == 0, "unsetenv(\"PCX_ABSENT\") returned %d (errno %s), expected 0", r, pc_errname(errno));
	pc_check(same_set(before, after), "the environment changed (%zu strings before, %zu after)",
		 before.n, after.n);
	pc_pass();
}
