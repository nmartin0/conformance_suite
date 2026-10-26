/*
 * confstr(_CS_V8_ENV) is a space-separated list of name=value pairs.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.confstr.006
 * MUTANT    confstr.env_no_value
 * MUTANT    confstr.env_bare_name
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	pc_begin("XSH.confstr.006");
#ifndef _CS_V8_ENV
	pc_unresolved("<unistd.h> does not define _CS_V8_ENV");
#else
	{
		char buf[8192], *tok, *save = NULL;
		size_t n;

		pc_test();
		n = confstr(_CS_V8_ENV, buf, sizeof buf);
		pc_check(n >= 1, "confstr(_CS_V8_ENV) has no value (errno %s)", pc_errname(errno));
		if (n > sizeof buf)
			pc_unresolved("the value of _CS_V8_ENV needs %zu bytes", n);
		for (tok = strtok_r(buf, " ", &save); tok != NULL; tok = strtok_r(NULL, " ", &save)) {
			char *eq = strchr(tok, '=');

			pc_check(eq != NULL && eq != tok, "_CS_V8_ENV holds \"%s\", not a name=value pair", tok);
		}
	}
	pc_pass();
#endif
}
