/*
 * bind_textdomain_codeset() returns the newly bound code set; a null codeset queries; the last call wins.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.textdomain.008
 */
#include "td.h"

int main(void)
{
	const char *cs[2];
	const char *r;
	int n;

	pc_begin("XSH.textdomain.008");
	n = td_codesets(cs);
	if (n == 0)
		pc_unresolved("none of the probed code set names is accepted by iconv_open");
	pc_test();
	r = bind_textdomain_codeset("pc-cs-dom", cs[0]);
	pc_check(r != NULL, "binding \"%s\" failed: %s", cs[0], pc_errname(errno));
	pc_check(strcmp(r, cs[0]) == 0, "returned \"%s\", expected \"%s\"", r, cs[0]);
	r = bind_textdomain_codeset("pc-cs-dom", NULL);
	pc_check(r != NULL && strcmp(r, cs[0]) == 0, "query returned \"%s\", expected \"%s\"", r ? r : "(null)", cs[0]);
	if (n > 1) {
		r = bind_textdomain_codeset("pc-cs-dom", cs[1]);
		pc_check(r != NULL && strcmp(r, cs[1]) == 0, "rebinding returned \"%s\", expected \"%s\"", r ? r : "(null)", cs[1]);
		r = bind_textdomain_codeset("pc-cs-dom", NULL);
		pc_check(r != NULL && strcmp(r, cs[1]) == 0, "query after rebinding returned \"%s\", expected \"%s\"",
		    r ? r : "(null)", cs[1]);
	}
	pc_pass();
}
