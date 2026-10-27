/*
 * bindtextdomain() returns a copy of the directory it was given; a query and a rebind agree.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.textdomain.005
 */
#include "td.h"

int main(void)
{
	char dir1[PATH_MAX + 16], dir2[PATH_MAX + 16], arg[PATH_MAX + 16];
	const char *r, *q, *r2;

	pc_begin("XSH.textdomain.005");
	(void)snprintf(dir1, sizeof dir1, "%s/b5a", td_cwd());
	(void)snprintf(dir2, sizeof dir2, "%s/b5b", td_cwd());
	pc_make_dir(dir1);
	pc_make_dir(dir2);
	pc_test();
	(void)snprintf(arg, sizeof arg, "%s", dir1);
	r = bindtextdomain("pc-bind-dom", arg);
	pc_check(r != NULL, "bindtextdomain failed: %s", pc_errname(errno));
	pc_check(strcmp(r, dir1) == 0, "returned \"%s\", expected \"%s\"", r, dir1);
	memset(arg, 'X', strlen(arg));
	pc_check(strcmp(r, dir1) == 0, "the returned string changed when the argument was overwritten: not a copy");
	q = bindtextdomain("pc-bind-dom", NULL);
	pc_check(q != NULL && strcmp(q, dir1) == 0, "query returned \"%s\", expected \"%s\"", q ? q : "(null)", dir1);
	r2 = bindtextdomain("pc-bind-dom", dir2);
	pc_check(r2 != NULL && strcmp(r2, dir2) == 0, "rebinding returned \"%s\", expected \"%s\"", r2 ? r2 : "(null)", dir2);
	q = bindtextdomain("pc-bind-dom", NULL);
	pc_check(q != NULL && strcmp(q, dir2) == 0, "query after rebinding returned \"%s\", expected \"%s\"", q ? q : "(null)", dir2);
	pc_pass();
}
