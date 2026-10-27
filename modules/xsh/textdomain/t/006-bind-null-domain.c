/*
 * bindtextdomain() with a null or empty domain makes no change, returns null, leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.textdomain.006
 */
#include "td.h"

int main(void)
{
	char dir[PATH_MAX + 16];
	const char *r;

	pc_begin("XSH.textdomain.006");
	(void)snprintf(dir, sizeof dir, "%s/b6", td_cwd());
	pc_make_dir(dir);
	pc_test();
	errno = EDOM;
	r = bindtextdomain(NULL, dir);
	pc_check(r == NULL, "bindtextdomain(NULL, dir) returned \"%s\"", r);
	pc_check(errno == EDOM, "bindtextdomain(NULL, dir) changed errno to %s", pc_errname(errno));
	errno = EDOM;
	r = bindtextdomain("", dir);
	pc_check(r == NULL, "bindtextdomain(\"\", dir) returned \"%s\"", r);
	pc_check(errno == EDOM, "bindtextdomain(\"\", dir) changed errno to %s", pc_errname(errno));
	errno = EDOM;
	r = bindtextdomain(NULL, NULL);
	pc_check(r == NULL, "bindtextdomain(NULL, NULL) returned \"%s\"", r);
	pc_check(errno == EDOM, "bindtextdomain(NULL, NULL) changed errno to %s", pc_errname(errno));
	pc_pass();
}
