/*
 * putenv() of a bare valid name removes the variable or fails with
 * EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.putenv.004
 * MUTANT    putenv.name_ignored
 * MUTANT    putenv.fails_and_removes
 * MUTANT    putenv.wrong_errno
 * VARIANT   putenv.name_einval
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
	static char s[] = "PCX_P";
	const char *p;
	int r, e;

	pc_begin("XSH.putenv.004");
	pc_check(setenv("PCX_P", "old", 1) == 0, "setenv failed: %s", pc_errname(errno));
	pc_test();
	errno = 0;
	r = putenv(s);
	e = errno;
	p = getenv("PCX_P");
	if (r == 0) {
		pc_check(p == NULL, "putenv(\"PCX_P\") returned 0 but getenv(\"PCX_P\") still finds \"%s\"", p);
		pc_check(count_named("PCX_P") == 0, "putenv(\"PCX_P\") returned 0 but environ still holds PCX_P");
	} else {
		pc_check(e == EINVAL, "putenv(\"PCX_P\") failed with errno %s; the alternative to removal is EINVAL",
			 pc_errname(e));
		pc_check(p != NULL && strcmp(p, "old") == 0, "putenv(\"PCX_P\") failed but PCX_P=old is gone");
	}
	pc_pass();
}
