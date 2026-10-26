/*
 * unsetenv() removes a variable and nothing else.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.unsetenv.001
 * MUTANT    unsetenv.noop
 * MUTANT    unsetenv.prefix_match
 * MUTANT    unsetenv.drops_last
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
	size_t i;
	int r;

	pc_begin("XSH.unsetenv.001");
	pc_check(setenv("PCX_U", "u", 1) == 0 && setenv("PCX_UV", "uv", 1) == 0 && setenv("PCX_V", "v", 1) == 0,
		 "setenv failed: %s", pc_errname(errno));
	pc_test();
	before = snapshot();
	r = unsetenv("PCX_U");
	after = snapshot();
	pc_check(r == 0, "unsetenv(\"PCX_U\") returned %d (errno %s), expected 0", r, pc_errname(errno));
	pc_check(getenv("PCX_U") == NULL, "getenv(\"PCX_U\") still finds the variable");
	pc_check(count_named("PCX_U") == 0, "environ still holds a string named PCX_U");
	pc_check(after.n + 1 == before.n, "the environment had %zu strings and has %zu after removing one",
		 before.n, after.n);
	for (i = 0; i < before.n; i++)
		if (strcmp(before.s[i], "PCX_U=u") != 0)
			pc_check(bsearch(&before.s[i], after.s, after.n, sizeof *after.s, by_string) != NULL,
				 "unsetenv(\"PCX_U\") also removed \"%s\"", before.s[i]);
	pc_pass();
}
