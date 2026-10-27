/*
 * Querying the code set of a domain never bound: a null pointer with errno unchanged, or a default name.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.textdomain.010
 */
#include "td.h"

int main(void)
{
	const char *r;

	pc_begin("XSH.textdomain.010");
	pc_test();
	errno = EDOM;
	r = bind_textdomain_codeset("pc-cs-unbound", NULL);
	if (r == NULL)
		pc_check(errno == EDOM, "a null result changed errno to %s", pc_errname(errno));
	else
		pc_check(r[0] != '\0', "the default code set is an empty string");
	pc_pass();
}
