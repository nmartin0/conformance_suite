/*
 * bind_textdomain_codeset() with a null or empty domain makes no change, returns null, leaves errno alone.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.textdomain.009
 */
#define TD_ICONV 1
#include "td.h"

int main(void)
{
	const char *cs[2];
	const char *r;

	pc_begin("XSH.textdomain.009");
	if (td_codesets(cs) == 0)
		pc_unresolved("none of the probed code set names is accepted by iconv_open");
	pc_test();
	errno = EDOM;
	r = bind_textdomain_codeset(NULL, cs[0]);
	pc_check(r == NULL, "bind_textdomain_codeset(NULL, cs) returned \"%s\"", r);
	pc_check(errno == EDOM, "bind_textdomain_codeset(NULL, cs) changed errno to %s", pc_errname(errno));
	errno = EDOM;
	r = bind_textdomain_codeset("", cs[0]);
	pc_check(r == NULL, "bind_textdomain_codeset(\"\", cs) returned \"%s\"", r);
	pc_check(errno == EDOM, "bind_textdomain_codeset(\"\", cs) changed errno to %s", pc_errname(errno));
	errno = EDOM;
	r = bind_textdomain_codeset(NULL, NULL);
	pc_check(r == NULL, "bind_textdomain_codeset(NULL, NULL) returned \"%s\"", r);
	pc_check(errno == EDOM, "bind_textdomain_codeset(NULL, NULL) changed errno to %s", pc_errname(errno));
	pc_pass();
}
