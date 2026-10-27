/*
 * basename("//") returns "/" or "//".
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.basename.003
 */
#include "pc.h"
#include <libgen.h>
#include <string.h>

int main(void)
{
	char buf[] = "//";
	char *r;

	pc_begin("XSH.basename.003");
	pc_test();
	r = basename(buf);
	pc_check(r != NULL && (strcmp(r, "/") == 0 || strcmp(r, "//") == 0), "basename(\"//\") returned \"%s\"", r != NULL ? r : "(null)");
	pc_pass();
}
