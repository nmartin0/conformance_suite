/*
 * setenv() with a non-zero overwrite replaces an existing variable.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setenv.002
 * MUTANT    setenv.duplicates
 * MUTANT    setenv.overwrite_one_only
 * VARIANT   setenv.front
 * VARIANT   setenv.fresh_array
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
	const char *p;
	int r1, r2;

	pc_begin("XSH.setenv.002");
	pc_check(setenv("PCX_N", "old", 1) == 0, "setenv failed: %s", pc_errname(errno));
	pc_check(count_string("PCX_N=old") == 1, "PCX_N=old is not in environ");
	pc_test();
	r1 = setenv("PCX_N", "new", 1);
	r2 = setenv("PCX_N", "newer", -1);
	pc_check(r1 == 0, "setenv(\"PCX_N\", \"new\", 1) returned %d, expected 0", r1);
	pc_check(r2 == 0, "setenv(\"PCX_N\", \"newer\", -1) returned %d, expected 0", r2);
	p = getenv("PCX_N");
	pc_check(p != NULL && strcmp(p, "newer") == 0, "getenv(\"PCX_N\") is %s%s%s, expected \"newer\"",
		 p ? "\"" : "", p ? p : "a null pointer", p ? "\"" : "");
	pc_check(count_named("PCX_N") == 1, "environ holds %d strings named PCX_N, expected one",
		 count_named("PCX_N"));
	pc_check(count_string("PCX_N=newer") == 1, "environ does not hold \"PCX_N=newer\"");
	pc_pass();
}
