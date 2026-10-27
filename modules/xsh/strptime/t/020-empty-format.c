/*
 * An empty format parses nothing and returns the start of the string.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strptime.020
 */
#include "sp.h"

int main(void)
{
	struct tm tm = sp_blank();
	const char *buf = "abc";
	const char *e = "";
	char *r;

	pc_begin("XSH.strptime.020");
	pc_test();
	r = strptime(buf, "", &tm);
	pc_check(r == buf, "strptime with an empty format returned offset %ld, expected 0", r ? (long)(r - buf) : -1L);
	r = strptime(e, "", &tm);
	pc_check(r == e, "strptime of an empty string with an empty format did not return its start");
	pc_pass();
}
