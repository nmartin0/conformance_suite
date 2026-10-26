/*
 * setenv() copies the name and value strings.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.setenv.004
 * MUTANT    setenv.keeps_value_pointer
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
	char name[16], value[16];
	const char *p;
	int r;

	pc_begin("XSH.setenv.004");
	(void)unsetenv("PCX_C");
	strcpy(name, "PCX_C");
	strcpy(value, "orig");
	pc_test();
	r = setenv(name, value, 1);
	pc_check(r == 0, "setenv(\"PCX_C\", \"orig\", 1) returned %d, expected 0", r);
	strcpy(name, "PCX_D");
	strcpy(value, "XXXX");
	p = getenv("PCX_C");
	pc_check(p != NULL && strcmp(p, "orig") == 0,
		 "after the caller's arrays were overwritten, getenv(\"PCX_C\") is %s%s%s, expected \"orig\"",
		 p ? "\"" : "", p ? p : "a null pointer", p ? "\"" : "");
	pc_check(count_string("PCX_C=orig") == 1, "environ no longer holds \"PCX_C=orig\"");
	pc_check(count_named("PCX_D") == 0, "environ holds a variable PCX_D (the caller's renamed array)");
	pc_pass();
}
