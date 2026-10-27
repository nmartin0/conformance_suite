/*
 * In the POSIX locale msgid is returned even though messages objects exist for the locale names C and POSIX.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gettext.007
 */
#include "gt.h"

int main(void)
{
	char dir[PATH_MAX + 8];
	const char *ln, *cs;
	static char one[] = GT_ONE;
	static char file[] = GT_FILE;
	static char files[] = GT_FILES;

	pc_begin("XSH.gettext.007");
	ln = setlocale(LC_MESSAGES, NULL);
	if (ln == NULL || (strcmp(ln, "C") != 0 && strcmp(ln, "POSIX") != 0))
		pc_unresolved("the LC_MESSAGES locale is not named C or POSIX");
	cs = nl_langinfo(CODESET);
	(void)unsetenv("NLSPATH");
	(void)setenv("LANGUAGE", "C:POSIX", 1);
	(void)snprintf(dir, sizeof dir, "%s/mo7", gt_cwd());
	gt_make_mo(dir, "C", "pc007", cs);
	gt_make_mo(dir, "POSIX", "pc007", cs);
	if (bindtextdomain("pc007", dir) == NULL)
		pc_unresolved("bindtextdomain failed");
	pc_test();
	pc_check(dgettext("pc007", one) == one, "dgettext returned a translation in the POSIX locale");
	pc_check(dcgettext("pc007", one, LC_MESSAGES) == one, "dcgettext returned a translation in the POSIX locale");
	pc_check(dngettext("pc007", file, files, 2UL) == files, "dngettext returned a translation in the POSIX locale");
	pc_check(dngettext("pc007", file, files, 1UL) == file, "dngettext n=1 returned a translation in the POSIX locale");
	pc_check(textdomain("pc007") != NULL, "textdomain failed");
	pc_check(gettext(one) == one, "gettext returned a translation in the POSIX locale");
	pc_check(ngettext(file, files, 5UL) == files, "ngettext returned a translation in the POSIX locale");
	pc_pass();
}
