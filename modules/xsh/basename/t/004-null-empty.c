/*
 * basename() of a null pointer or an empty string returns ".".
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.basename.004
 */
#include "pc.h"
#include <libgen.h>
#include <string.h>

int main(void)
{
	char buf[] = "";
	char *r;

	pc_begin("XSH.basename.004");
	pc_test();
	r = basename(NULL);
	pc_check(r != NULL && strcmp(r, ".") == 0, "basename(NULL) returned \"%s\"", r != NULL ? r : "(null)");
	r = basename(buf);
	pc_check(r != NULL && strcmp(r, ".") == 0, "basename(\"\") returned \"%s\"", r != NULL ? r : "(null)");
	pc_pass();
}
