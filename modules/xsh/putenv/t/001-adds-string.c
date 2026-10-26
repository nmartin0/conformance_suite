/*
 * putenv() makes the caller's string part of the environment.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.putenv.001
 * MUTANT    putenv.copies
 * MUTANT    putenv.returns_one
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

/* 1 if the pointer s itself is in environ. */
static int holds_pointer(const char *s)
{
	size_t i;

	for (i = 0; environ[i] != NULL; i++)
		if (environ[i] == s)
			return 1;
	return 0;
}

int main(void)
{
	static char s[] = "PCX_P=one";
	const char *p;
	int r;

	pc_begin("XSH.putenv.001");
	(void)unsetenv("PCX_P");
	pc_test();
	r = putenv(s);
	pc_check(r == 0, "putenv(\"PCX_P=one\") returned %d (errno %s), expected 0", r, pc_errname(errno));
	p = getenv("PCX_P");
	pc_check(p != NULL && strcmp(p, "one") == 0, "getenv(\"PCX_P\") is %s%s%s, expected \"one\"",
		 p ? "\"" : "", p ? p : "a null pointer", p ? "\"" : "");
	pc_check(holds_pointer(s), "environ does not hold the string passed to putenv() (a copy?)");
	pc_check(count_named("PCX_P") == 1, "environ holds %d strings named PCX_P", count_named("PCX_P"));
	pc_pass();
}
