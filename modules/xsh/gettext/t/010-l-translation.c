/*
 * The _l functions use the given locale object, not the global locale, to find the translation.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gettext.010
 */
#include "gt.h"

int main(void)
{
	char dir[PATH_MAX + 8], ln[128], cs[64];
	static char one[] = GT_ONE;
	static char file[] = GT_FILE;
	static char files[] = GT_FILES;
	locale_t lo;
	const char *r;

	pc_begin("XSH.gettext.010");
	gt_pick_locale(ln, sizeof ln, cs, sizeof cs);
	(void)unsetenv("NLSPATH");
	(void)unsetenv("LANGUAGE");
	(void)snprintf(dir, sizeof dir, "%s/mo10", gt_cwd());
	gt_make_mo(dir, ln, "pc010", cs);
	lo = newlocale(LC_ALL_MASK, ln, (locale_t)0);
	if (lo == (locale_t)0)
		pc_unresolved("newlocale for the found locale failed");
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale back to the POSIX locale failed");
	if (bindtextdomain("pc010", dir) == NULL || textdomain("pc010") == NULL)
		pc_unresolved("bindtextdomain or textdomain failed");
	pc_test();
	pc_check(gettext(one) == one, "with the global POSIX locale gettext translated");
	r = gettext_l(one, lo);
	pc_check(strcmp(r, GT_ONE_T) == 0, "gettext_l gave \"%s\", expected \"%s\"", r, GT_ONE_T);
	r = dgettext_l("pc010", one, lo);
	pc_check(strcmp(r, GT_ONE_T) == 0, "dgettext_l gave \"%s\", expected \"%s\"", r, GT_ONE_T);
	r = dcgettext_l("pc010", one, LC_MESSAGES, lo);
	pc_check(strcmp(r, GT_ONE_T) == 0, "dcgettext_l gave \"%s\", expected \"%s\"", r, GT_ONE_T);
	r = ngettext_l(file, files, 2UL, lo);
	pc_check(strcmp(r, GT_PL1) == 0, "ngettext_l n=2 gave \"%s\", expected \"%s\"", r, GT_PL1);
	r = dngettext_l("pc010", file, files, 1UL, lo);
	pc_check(strcmp(r, GT_PL0) == 0, "dngettext_l n=1 gave \"%s\", expected \"%s\"", r, GT_PL0);
	r = dcngettext_l("pc010", file, files, 6UL, LC_MESSAGES, lo);
	pc_check(strcmp(r, GT_PL2) == 0, "dcngettext_l n=6 gave \"%s\", expected \"%s\"", r, GT_PL2);
	freelocale(lo);
	pc_pass();
}
