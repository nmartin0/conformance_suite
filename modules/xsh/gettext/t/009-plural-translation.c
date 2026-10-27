/*
 * In a named locale the plural functions pick the form the object's plural rule selects.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gettext.009
 */
#include "gt.h"

static void want(const char *got, const char *exp, const char *what, unsigned long n)
{
	pc_check(strcmp(got, exp) == 0, "%s with n=%lu gave \"%s\", expected \"%s\"", what, n, got, exp);
}

int main(void)
{
	char dir[PATH_MAX + 8], ln[128], cs[64];
	static char file[] = GT_FILE;
	static char files[] = GT_FILES;
	static char xs[] = "pc x item";
	static char xp[] = "pc x items";

	pc_begin("XSH.gettext.009");
	gt_pick_locale(ln, sizeof ln, cs, sizeof cs);
	(void)unsetenv("NLSPATH");
	(void)unsetenv("LANGUAGE");
	(void)snprintf(dir, sizeof dir, "%s/mo9", gt_cwd());
	gt_make_mo(dir, ln, "pc009", cs);
	if (bindtextdomain("pc009", dir) == NULL || textdomain("pc009") == NULL)
		pc_unresolved("bindtextdomain or textdomain failed");
	pc_test();
	want(ngettext(file, files, 1UL), GT_PL0, "ngettext", 1);
	want(ngettext(file, files, 2UL), GT_PL1, "ngettext", 2);
	want(ngettext(file, files, 5UL), GT_PL2, "ngettext", 5);
	want(ngettext(file, files, 0UL), GT_PL2, "ngettext", 0);
	want(dngettext("pc009", file, files, 2UL), GT_PL1, "dngettext", 2);
	want(dcngettext("pc009", file, files, 1UL, LC_MESSAGES), GT_PL0, "dcngettext", 1);
	want(dcngettext("pc009", file, files, 9UL, LC_MESSAGES), GT_PL2, "dcngettext", 9);
	pc_check(ngettext(xs, xp, 1UL) == xs, "a msgid missing from the object with n=1 did not return msgid");
	pc_check(ngettext(xs, xp, 3UL) == xp, "a msgid missing from the object with n=3 did not return msgid_plural");
	pc_pass();
}
