/*
 * In the POSIX locale gettext() returns msgid itself.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gettext.001
 */
#include "gt.h"

int main(void)
{
	static char m1[] = "pc plain message";
	static char m2[] = "pc another message %d";
	static char m3[] = "pc  odd   spacing";
	char *r;

	pc_begin("XSH.gettext.001");
	pc_test();
	r = gettext(m1);
	pc_check(r == m1, "gettext(\"%s\") did not return msgid", m1);
	r = gettext(m2);
	pc_check(r == m2, "gettext(\"%s\") did not return msgid", m2);
	r = gettext(m3);
	pc_check(r == m3, "gettext(\"%s\") did not return msgid", m3);
	/* after naming a text domain nothing changes */
	pc_check(textdomain("pc-gettext-001") != NULL, "textdomain failed");
	r = gettext(m1);
	pc_check(r == m1, "after textdomain(): gettext(\"%s\") did not return msgid", m1);
	pc_pass();
}
