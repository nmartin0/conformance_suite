/*
 * Altering a string given to putenv() changes the environment.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.putenv.002
 * MUTANT    putenv.copies
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

int main(void)
{
	static char s[] = "PCX_P=one";
	const char *p;
	int r;

	pc_begin("XSH.putenv.002");
	(void)unsetenv("PCX_P");
	(void)unsetenv("PCX_R");
	pc_test();
	r = putenv(s);
	pc_check(r == 0, "putenv(\"PCX_P=one\") returned %d (errno %s), expected 0", r, pc_errname(errno));
	memcpy(s + 6, "two", 3);
	p = getenv("PCX_P");
	pc_check(p != NULL && strcmp(p, "two") == 0,
		 "after the string was changed to \"PCX_P=two\", getenv(\"PCX_P\") is %s%s%s",
		 p ? "\"" : "", p ? p : "a null pointer", p ? "\"" : "");
	memcpy(s, "PCX_R=new", sizeof s);
	p = getenv("PCX_R");
	pc_check(p != NULL && strcmp(p, "new") == 0,
		 "after the string was changed to \"PCX_R=new\", getenv(\"PCX_R\") is %s%s%s",
		 p ? "\"" : "", p ? p : "a null pointer", p ? "\"" : "");
	pc_check(getenv("PCX_P") == NULL, "after the string was renamed to PCX_R, getenv(\"PCX_P\") still finds a value");
	pc_pass();
}
