/*
 * Before any call to set a domain, the current text domain is "messages".
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.textdomain.001
 */
#include "td.h"

int main(void)
{
	const char *r;

	pc_begin("XSH.textdomain.001");
	pc_test();
	r = textdomain(NULL);
	pc_check(r != NULL, "textdomain(NULL) returned a null pointer");
	pc_check(strcmp(r, "messages") == 0, "initial text domain is \"%s\", expected \"messages\"", r);
	pc_pass();
}
