/*
 * getenv() returns a pointer into the environment data environ points to.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getenv.002
 * MUTANT    getenv.copies
 * VARIANT   getenv.copies_on_reassign
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

/* 1 if p lies within one of the strings of environ (its null byte included). */
static int in_environ(const char *p)
{
	size_t i;

	for (i = 0; environ[i] != NULL; i++)
		if (p >= environ[i] && p <= environ[i] + strlen(environ[i]))
			return 1;
	return 0;
}

int main(void)
{
	const char *p;

	pc_begin("XSH.getenv.002");
	pc_check(setenv("PCX_A", "one", 1) == 0, "setenv failed: %s", pc_errname(errno));
	pc_test();
	p = getenv("PCX_A");
	pc_check(p != NULL, "getenv(\"PCX_A\") is a null pointer after setenv()");
	pc_check(strcmp(p, "one") == 0, "getenv(\"PCX_A\") is \"%s\", expected \"one\"", p);
	pc_check(in_environ(p), "getenv(\"PCX_A\") points outside the strings of environ (a copy)");
	pc_pass();
}
