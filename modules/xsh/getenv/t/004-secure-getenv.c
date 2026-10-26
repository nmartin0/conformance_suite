/*
 * secure_getenv() behaves as getenv() where the IDs were equal at startup
 * (unless an implementation-defined criterion applies).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getenv.004
 * MUTANT    getenv.secure_copies
 * MUTANT    getenv.secure_empty_for_missing
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char **environ;

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

	pc_begin("XSH.getenv.004");
	pc_check(setenv("PCX_A", "one", 1) == 0, "setenv failed: %s", pc_errname(errno));
	(void)unsetenv("PCX_Z");
	pc_test();
	pc_check(secure_getenv("PCX_Z") == NULL, "secure_getenv(\"PCX_Z\") (not in the environment) is not a null pointer");
	p = secure_getenv("PCX_A");
	if (p == NULL) {
		if (getuid() != geteuid() || getgid() != getegid())
			pc_unresolved("the test runs with real and effective IDs that differ");
		pc_unresolved("secure_getenv(\"PCX_A\") is a null pointer although the real and effective IDs are "
			      "equal: allowed only by an implementation-defined criterion (see the conformance document)");
	}
	pc_check(strcmp(p, "one") == 0, "secure_getenv(\"PCX_A\") is \"%s\", expected \"one\"", p);
	pc_check(in_environ(p), "secure_getenv(\"PCX_A\") points outside the strings of environ (getenv() may not)");
	pc_pass();
}
