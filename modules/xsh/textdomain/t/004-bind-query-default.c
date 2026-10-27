/*
 * bindtextdomain() with a null directory on an unbound domain returns the default directory.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.textdomain.004
 */
#include "td.h"

int main(void)
{
	const char *r;

	pc_begin("XSH.textdomain.004");
	pc_test();
	r = bindtextdomain("pc-unbound-dom", NULL);
	pc_check(r != NULL, "bindtextdomain(domain, NULL) on an unbound domain returned a null pointer");
	pc_check(r[0] != '\0', "the default directory is an empty string");
	pc_pass();
}
