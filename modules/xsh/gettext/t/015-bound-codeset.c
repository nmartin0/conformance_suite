/*
 * A code set bound with bind_textdomain_codeset() is the output code set, not the locale's.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gettext.015
 */
#include "gt.h"

int main(void)
{
	char dir[PATH_MAX + 8], ln[128], cs[64];
	static char cafe[] = GT_CAFE;
	iconv_t cd;
	const char *r;

	pc_begin("XSH.gettext.015");
	gt_pick_locale(ln, sizeof ln, cs, sizeof cs);
	if (strcmp(cs, "UTF-8") != 0)
		pc_unresolved("the locale's code set is \"%s\", not UTF-8", cs);
	cd = iconv_open("ISO-8859-1", "UTF-8");
	if (cd == (iconv_t)-1)
		pc_unresolved("no conversion from UTF-8 to ISO-8859-1");
	(void)iconv_close(cd);
	(void)unsetenv("NLSPATH");
	(void)unsetenv("LANGUAGE");
	(void)snprintf(dir, sizeof dir, "%s/mo15", gt_cwd());
	/* the object holds U+00E9 in UTF-8 (0xC3 0xA9) */
	gt_make_mo_cafe(dir, ln, "pc015", "UTF-8", "pc caf\xc3\xa9");
	if (bindtextdomain("pc015", dir) == NULL || textdomain("pc015") == NULL)
		pc_unresolved("bindtextdomain or textdomain failed");
	if (bind_textdomain_codeset("pc015", "ISO-8859-1") == NULL)
		pc_unresolved("bind_textdomain_codeset failed");
	pc_test();
	r = gettext(cafe);
	pc_check(strcmp(r, "pc caf\xe9") == 0, "the translation was not converted to the bound code set (got %lu bytes)",
	    (unsigned long)strlen(r));
	pc_pass();
}
