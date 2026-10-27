/*
 * In a named locale with a bound directory, gettext() and the d/dc forms give the translation.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gettext.008
 */
#include "gt.h"

int main(void)
{
	char dir[PATH_MAX + 8], ln[128], cs[64];
	static char one[] = GT_ONE;
	static char miss[] = "pc not in the object";
	const char *r;

	pc_begin("XSH.gettext.008");
	gt_pick_locale(ln, sizeof ln, cs, sizeof cs);
	(void)unsetenv("NLSPATH");
	(void)unsetenv("LANGUAGE");
	(void)snprintf(dir, sizeof dir, "%s/mo8", gt_cwd());
	gt_make_mo(dir, ln, "pc008", cs);
	if (bindtextdomain("pc008", dir) == NULL || textdomain("pc008") == NULL)
		pc_unresolved("bindtextdomain or textdomain failed");
	pc_test();
	r = gettext(one);
	pc_check(strcmp(r, GT_ONE_T) == 0, "gettext gave \"%s\", expected \"%s\"", r, GT_ONE_T);
	r = dgettext("pc008", one);
	pc_check(strcmp(r, GT_ONE_T) == 0, "dgettext gave \"%s\", expected \"%s\"", r, GT_ONE_T);
	r = dcgettext("pc008", one, LC_MESSAGES);
	pc_check(strcmp(r, GT_ONE_T) == 0, "dcgettext gave \"%s\", expected \"%s\"", r, GT_ONE_T);
	r = gettext(miss);
	pc_check(r == miss, "a msgid missing from the object did not return msgid");
	r = dgettext("pc008-none", one);
	pc_check(r == one, "a domain without an object did not return msgid");
	pc_pass();
}
