/*
 * The current text domain serves gettext(); the domain argument of dgettext() overrides it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gettext.011
 */
#include "gt.h"

int main(void)
{
	char dir[PATH_MAX + 8], ln[128], cs[64];
	static char one[] = GT_ONE;
	const char *r;

	pc_begin("XSH.gettext.011");
	gt_pick_locale(ln, sizeof ln, cs, sizeof cs);
	(void)unsetenv("NLSPATH");
	(void)unsetenv("LANGUAGE");
	(void)snprintf(dir, sizeof dir, "%s/mo11", gt_cwd());
	gt_make_mo(dir, ln, "pc011a", cs);
	if (bindtextdomain("pc011a", dir) == NULL || bindtextdomain("pc011b", dir) == NULL)
		pc_unresolved("bindtextdomain failed");
	pc_test();
	pc_check(textdomain("pc011b") != NULL, "textdomain(pc011b) failed");
	pc_check(gettext(one) == one, "gettext found a translation in a domain with no object");
	r = dgettext("pc011a", one);
	pc_check(strcmp(r, GT_ONE_T) == 0, "dgettext(pc011a) gave \"%s\", expected \"%s\"", r, GT_ONE_T);
	pc_check(textdomain("pc011a") != NULL, "textdomain(pc011a) failed");
	r = gettext(one);
	pc_check(strcmp(r, GT_ONE_T) == 0, "gettext gave \"%s\", expected \"%s\"", r, GT_ONE_T);
	pc_check(dgettext("pc011b", one) == one, "dgettext(pc011b) found a translation in a domain with no object");
	pc_check(dgettext(NULL, one) != one && strcmp(dgettext(NULL, one), GT_ONE_T) == 0,
	    "dgettext with a null domain did not use the current domain");
	pc_pass();
}
