/*
 * getenv() searches the array environ points to now, after the
 * application assigns environ a new array.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getenv.003
 * MUTANT    getenv.cached_table
 * VARIANT   getenv.copies_on_reassign
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

/* A new NULL-terminated array: environ's strings without PCX_*, plus extra. */
static char **with(const char *extra)
{
	size_t n, i, k;
	char **a;

	for (n = 0; environ[n] != NULL; n++)
		;
	a = malloc((n + 2) * sizeof *a);
	if (a == NULL)
		return NULL;
	for (i = k = 0; i < n; i++)
		if (strncmp(environ[i], "PCX_", 4) != 0)
			a[k++] = environ[i];
	a[k++] = (char *)extra;
	a[k] = NULL;
	return a;
}

int main(void)
{
	static char n_new[] = "PCX_N=new";
	static char a_again[] = "PCX_A=again";
	char **first, **second;
	const char *p;

	pc_begin("XSH.getenv.003");
	pc_check(setenv("PCX_A", "one", 1) == 0, "setenv failed: %s", pc_errname(errno));
	first = with(n_new);
	second = with(a_again);
	pc_check(first != NULL && second != NULL, "out of memory");
	pc_test();
	p = getenv("PCX_A");
	pc_check(p != NULL && strcmp(p, "one") == 0, "getenv(\"PCX_A\") is not \"one\" after setenv()");
	environ = first;
	p = getenv("PCX_N");
	pc_check(p != NULL, "after environ was assigned an array holding PCX_N=new, getenv(\"PCX_N\") is a null pointer");
	pc_check(strcmp(p, "new") == 0, "getenv(\"PCX_N\") is \"%s\", expected \"new\"", p);
	p = getenv("PCX_A");
	pc_check(p == NULL, "after environ was assigned an array without PCX_A, getenv(\"PCX_A\") is \"%s\"", p);
	environ = second;
	p = getenv("PCX_A");
	pc_check(p != NULL && strcmp(p, "again") == 0,
		 "after environ was assigned an array holding PCX_A=again, getenv(\"PCX_A\") is %s%s%s",
		 p ? "\"" : "", p ? p : "a null pointer", p ? "\"" : "");
	pc_check(getenv("PCX_N") == NULL, "getenv(\"PCX_N\") still finds the variable of the previous array");
	pc_pass();
}
