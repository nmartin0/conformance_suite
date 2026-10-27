/*
 * [XSI] NLSPATH templates locate the messages object, %N standing for the domain.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gettext.013
 */
#include "gt.h"

int main(void)
{
	char dir[PATH_MAX + 8], ln[128], cs[64], path[2 * PATH_MAX + 64];
	static char one[] = GT_ONE;
	const char *r;

	pc_begin("XSH.gettext.013");
	gt_pick_locale(ln, sizeof ln, cs, sizeof cs);
	(void)unsetenv("LANGUAGE");
	(void)snprintf(dir, sizeof dir, "%s/mo13", gt_cwd());
	gt_make_mo(dir, "pcnls", "pc013a", cs);
	gt_make_mo(dir, "pcnls", "pc013b", cs);
	pc_test();
	/* one template */
	(void)snprintf(path, sizeof path, "%s/pcnls/LC_MESSAGES/%%N.mo", dir);
	pc_check(setenv("NLSPATH", path, 1) == 0, "setenv NLSPATH failed");
	r = dgettext("pc013a", one);
	pc_check(strcmp(r, GT_ONE_T) == 0, "with one NLSPATH template dgettext gave \"%s\", expected \"%s\"", r, GT_ONE_T);
	/* the first template finds nothing, the second does */
	(void)snprintf(path, sizeof path, "%s/pcnone/%%N.mo:%s/pcnls/LC_MESSAGES/%%N.mo", dir, dir);
	pc_check(setenv("NLSPATH", path, 1) == 0, "setenv NLSPATH failed");
	r = dgettext("pc013b", one);
	pc_check(strcmp(r, GT_ONE_T) == 0, "with two NLSPATH templates dgettext gave \"%s\", expected \"%s\"", r, GT_ONE_T);
	pc_pass();
}
