/*
 * setenv() works on an environment array the application assigned to
 * environ, without writing past its end.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setenv.006
 * MUTANT    setenv.appends_in_place
 * MUTANT    setenv.own_copy
 * VARIANT   setenv.front
 * VARIANT   setenv.fresh_array
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

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
	static char kept[] = "PCX_K=kept";
	static char guard[] = "PCX_G=guard";
	char **mine;
	size_t n, i, k;
	const char *p;
	int r;

	pc_begin("XSH.setenv.006");
	(void)unsetenv("PCX_N");
	for (n = 0; environ[n] != NULL; n++)
		;
	/* the old strings, PCX_K, the terminating null pointer, then a guard slot */
	mine = malloc((n + 3) * sizeof *mine);
	pc_check(mine != NULL, "out of memory");
	for (i = k = 0; i < n; i++)
		if (strncmp(environ[i], "PCX_", 4) != 0)
			mine[k++] = environ[i];
	mine[k++] = kept;
	mine[k] = NULL;
	mine[k + 1] = guard;
	pc_test();
	environ = mine;
	r = setenv("PCX_N", "val", 1);
	pc_check(r == 0, "setenv(\"PCX_N\", \"val\", 1) returned %d (errno %s), expected 0", r, pc_errname(errno));
	pc_check(mine[k + 1] == guard, "setenv() wrote past the null pointer that ends the application's array");
	pc_check(count_string("PCX_N=val") == 1, "environ does not hold \"PCX_N=val\"");
	pc_check(count_string("PCX_K=kept") == 1, "environ lost \"PCX_K=kept\" of the application's array");
	p = getenv("PCX_N");
	pc_check(p != NULL && strcmp(p, "val") == 0, "getenv(\"PCX_N\") is not \"val\"");
	p = getenv("PCX_K");
	pc_check(p != NULL && strcmp(p, "kept") == 0, "getenv(\"PCX_K\") is not \"kept\"");
	pc_pass();
}
