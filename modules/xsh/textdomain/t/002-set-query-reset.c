/*
 * textdomain() sets the domain and returns it; a null argument queries; an empty string resets to "messages".
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.textdomain.002
 */
#include "td.h"

int main(void)
{
	const char *r;

	pc_begin("XSH.textdomain.002");
	pc_test();
	r = textdomain("pc-dom-two");
	pc_check(r != NULL && strcmp(r, "pc-dom-two") == 0, "setting the domain returned \"%s\"", r ? r : "(null)");
	r = textdomain(NULL);
	pc_check(r != NULL && strcmp(r, "pc-dom-two") == 0, "query returned \"%s\", expected \"pc-dom-two\"", r ? r : "(null)");
	r = textdomain(NULL);
	pc_check(r != NULL && strcmp(r, "pc-dom-two") == 0, "a second query changed the domain to \"%s\"", r ? r : "(null)");
	r = textdomain("pc-dom-three");
	pc_check(r != NULL && strcmp(r, "pc-dom-three") == 0, "setting another domain returned \"%s\"", r ? r : "(null)");
	r = textdomain("");
	pc_check(r != NULL && strcmp(r, "messages") == 0, "textdomain(\"\") returned \"%s\", expected \"messages\"", r ? r : "(null)");
	r = textdomain(NULL);
	pc_check(r != NULL && strcmp(r, "messages") == 0, "after the empty string the domain is \"%s\"", r ? r : "(null)");
	pc_pass();
}
