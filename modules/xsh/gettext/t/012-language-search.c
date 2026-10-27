/*
 * LANGUAGE names locale directories to search, in order, before the current locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gettext.012
 */
#include "gt.h"

int main(void)
{
	char dir[PATH_MAX + 8], ln[128], cs[64];
	static char one[] = GT_ONE;
	static char file[] = GT_FILE;
	static char files[] = GT_FILES;
	const char *r;

	pc_begin("XSH.gettext.012");
	gt_pick_locale(ln, sizeof ln, cs, sizeof cs);
	(void)unsetenv("NLSPATH");
	(void)unsetenv("LANGUAGE");
	(void)snprintf(dir, sizeof dir, "%s/mo12", gt_cwd());
	/* the only object is under a locale name that is not the current one */
	gt_make_mo(dir, "pcxx", "pc012", cs);
	if (bindtextdomain("pc012", dir) == NULL || textdomain("pc012") == NULL)
		pc_unresolved("bindtextdomain or textdomain failed");
	pc_test();
	pc_check(gettext(one) == one, "found a translation without LANGUAGE although the current locale has no object");
	pc_check(setenv("LANGUAGE", "pcnone:pcxx", 1) == 0, "setenv LANGUAGE failed");
	r = gettext(one);
	pc_check(strcmp(r, GT_ONE_T) == 0, "with LANGUAGE=pcnone:pcxx gettext gave \"%s\", expected \"%s\"", r, GT_ONE_T);
	r = ngettext(file, files, 2UL);
	pc_check(strcmp(r, GT_PL1) == 0, "with LANGUAGE=pcnone:pcxx ngettext n=2 gave \"%s\", expected \"%s\"", r, GT_PL1);
	pc_pass();
}
