/*
 * putenv() replaces an existing variable with the caller's string.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.putenv.003
 * MUTANT    putenv.duplicates
 * MUTANT    putenv.keeps_old
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

int main(void)
{
	static char s[] = "PCX_P=new";
	const char *p;
	size_t i;
	int r, found = 0;

	pc_begin("XSH.putenv.003");
	pc_check(setenv("PCX_P", "old", 1) == 0, "setenv failed: %s", pc_errname(errno));
	pc_test();
	r = putenv(s);
	pc_check(r == 0, "putenv(\"PCX_P=new\") returned %d (errno %s), expected 0", r, pc_errname(errno));
	pc_check(count_named("PCX_P") == 1, "environ holds %d strings named PCX_P, expected one",
		 count_named("PCX_P"));
	for (i = 0; environ[i] != NULL; i++)
		if (environ[i] == s)
			found = 1;
	pc_check(found, "environ does not hold the string passed to putenv()");
	p = getenv("PCX_P");
	pc_check(p != NULL && strcmp(p, "new") == 0, "getenv(\"PCX_P\") is %s%s%s, expected \"new\"",
		 p ? "\"" : "", p ? p : "a null pointer", p ? "\"" : "");
	pc_pass();
}
