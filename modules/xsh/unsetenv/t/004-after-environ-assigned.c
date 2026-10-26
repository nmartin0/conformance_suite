/*
 * unsetenv() removes a variable from an environment array the
 * application assigned to environ.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.unsetenv.004
 * MUTANT    unsetenv.own_copy
 * MUTANT    unsetenv.noop
 * VARIANT   unsetenv.swap_last
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

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
	static char u[] = "PCX_U=mine";
	static char kept[] = "PCX_K=kept";
	char **mine;
	size_t n, i, k;
	const char *p;
	int r;

	pc_begin("XSH.unsetenv.004");
	for (n = 0; environ[n] != NULL; n++)
		;
	mine = malloc((n + 3) * sizeof *mine);
	pc_check(mine != NULL, "out of memory");
	for (i = k = 0; i < n; i++)
		if (strncmp(environ[i], "PCX_", 4) != 0)
			mine[k++] = environ[i];
	mine[k++] = u;
	mine[k++] = kept;
	mine[k] = NULL;
	pc_test();
	environ = mine;
	r = unsetenv("PCX_U");
	pc_check(r == 0, "unsetenv(\"PCX_U\") returned %d (errno %s), expected 0", r, pc_errname(errno));
	pc_check(count_named("PCX_U") == 0, "environ still holds a string named PCX_U");
	pc_check(count_string("PCX_K=kept") == 1, "environ lost \"PCX_K=kept\"");
	pc_check(getenv("PCX_U") == NULL, "getenv(\"PCX_U\") still finds the variable");
	p = getenv("PCX_K");
	pc_check(p != NULL && strcmp(p, "kept") == 0, "getenv(\"PCX_K\") is not \"kept\"");
	pc_pass();
}
