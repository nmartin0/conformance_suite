/*
 * setenv() adds a variable that is not in the environment.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setenv.001
 * MUTANT    setenv.not_added
 * MUTANT    setenv.drops_entry
 * MUTANT    setenv.no_overwrite_means_no_add
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

/* 1 if every string of a is in b. */
static int subset(struct snap a, struct snap b)
{
	size_t i;

	for (i = 0; i < a.n; i++)
		if (bsearch(&a.s[i], b.s, b.n, sizeof *b.s, by_string) == NULL)
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
	struct snap before, after;
	const char *p;
	int r;

	pc_begin("XSH.setenv.001");
	(void)unsetenv("PCX_N");
	pc_check(count_named("PCX_N") == 0, "PCX_N is still in the environment");
	pc_test();
	before = snapshot();
	r = setenv("PCX_N", "val", 0);
	after = snapshot();
	pc_check(r == 0, "setenv(\"PCX_N\", \"val\", 0) returned %d (errno %s), expected 0", r, pc_errname(errno));
	p = getenv("PCX_N");
	pc_check(p != NULL && strcmp(p, "val") == 0, "getenv(\"PCX_N\") is %s%s%s, expected \"val\"",
		 p ? "\"" : "", p ? p : "a null pointer", p ? "\"" : "");
	pc_check(count_string("PCX_N=val") == 1 && count_named("PCX_N") == 1,
		 "environ holds %d strings named PCX_N, %d of them \"PCX_N=val\"; expected exactly one",
		 count_named("PCX_N"), count_string("PCX_N=val"));
	pc_check(subset(before, after), "a string of the environment was lost when PCX_N was added");
	pc_check(after.n == before.n + 1, "the environment had %zu strings and has %zu after adding one",
		 before.n, after.n);
	pc_pass();
}
