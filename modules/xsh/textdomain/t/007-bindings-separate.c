/*
 * Each text domain keeps its own directory binding.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.textdomain.007
 */
#include "td.h"

int main(void)
{
	char da[PATH_MAX + 16], db[PATH_MAX + 16];
	const char *r;

	pc_begin("XSH.textdomain.007");
	(void)snprintf(da, sizeof da, "%s/b7a", td_cwd());
	(void)snprintf(db, sizeof db, "%s/b7b", td_cwd());
	pc_make_dir(da);
	pc_make_dir(db);
	pc_check(bindtextdomain("pc-dom-a", da) != NULL, "binding the first domain failed: %s", pc_errname(errno));
	pc_check(bindtextdomain("pc-dom-b", db) != NULL, "binding the second domain failed: %s", pc_errname(errno));
	pc_test();
	r = bindtextdomain("pc-dom-a", NULL);
	pc_check(r != NULL && strcmp(r, da) == 0, "first domain is bound to \"%s\", expected \"%s\"", r ? r : "(null)", da);
	r = bindtextdomain("pc-dom-b", NULL);
	pc_check(r != NULL && strcmp(r, db) == 0, "second domain is bound to \"%s\", expected \"%s\"", r ? r : "(null)", db);
	pc_pass();
}
