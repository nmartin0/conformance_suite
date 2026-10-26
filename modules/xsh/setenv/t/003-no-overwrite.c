/*
 * setenv() with overwrite 0 leaves an existing variable as it is.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setenv.003
 * MUTANT    setenv.ignores_overwrite
 * MUTANT    setenv.no_overwrite_fails
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

int main(void)
{
	struct snap before, after;
	const char *p;
	int r;

	pc_begin("XSH.setenv.003");
	pc_check(setenv("PCX_N", "old", 1) == 0, "setenv failed: %s", pc_errname(errno));
	pc_test();
	before = snapshot();
	r = setenv("PCX_N", "new", 0);
	after = snapshot();
	pc_check(r == 0, "setenv(\"PCX_N\", \"new\", 0) returned %d (errno %s), expected 0", r, pc_errname(errno));
	p = getenv("PCX_N");
	pc_check(p != NULL && strcmp(p, "old") == 0, "getenv(\"PCX_N\") is %s%s%s, expected \"old\"",
		 p ? "\"" : "", p ? p : "a null pointer", p ? "\"" : "");
	pc_check(same_set(before, after), "the environment changed (%zu strings before, %zu after)",
		 before.n, after.n);
	pc_pass();
}
