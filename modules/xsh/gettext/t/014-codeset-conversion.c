/*
 * A translation in another code set is converted to the code set of the locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gettext.014
 */
#include "gt.h"

int main(void)
{
	char dir[PATH_MAX + 8], ln[128], cs[64];
	static char cafe[] = GT_CAFE;
	iconv_t cd;
	const char *r;

	pc_begin("XSH.gettext.014");
	gt_pick_locale(ln, sizeof ln, cs, sizeof cs);
	if (strcmp(cs, "UTF-8") != 0)
		pc_unresolved("the locale's code set is \"%s\", not UTF-8", cs);
	cd = iconv_open(cs, "ISO-8859-1");
	if (cd == (iconv_t)-1)
		pc_unresolved("no conversion from ISO-8859-1 to the code set of the locale");
	(void)iconv_close(cd);
	(void)unsetenv("NLSPATH");
	(void)unsetenv("LANGUAGE");
	(void)snprintf(dir, sizeof dir, "%s/mo14", gt_cwd());
	/* the object holds the character U+00E9 as the single ISO 8859-1 byte 0xE9 */
	gt_make_mo_cafe(dir, ln, "pc014", "ISO-8859-1", "pc caf\xe9");
	if (bindtextdomain("pc014", dir) == NULL || textdomain("pc014") == NULL)
		pc_unresolved("bindtextdomain or textdomain failed");
	pc_test();
	r = gettext(cafe);
	pc_check(strcmp(r, "pc caf\xc3\xa9") == 0, "the translation was not converted to UTF-8 (got %lu bytes)",
	    (unsigned long)strlen(r));
	pc_pass();
}
