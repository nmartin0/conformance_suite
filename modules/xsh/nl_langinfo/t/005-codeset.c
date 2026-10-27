/*
 * The code set name is not empty.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.nl_langinfo.005
 */
#include "lc.h"

int main(void)
{
	const char *p;

	pc_begin("XSH.nl_langinfo.005");
	lc_init();
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	pc_test();
	p = nl_langinfo(CODESET);
	pc_check(p != NULL, "nl_langinfo(CODESET) returned a null pointer");
	pc_check(p[0] != '\0', "nl_langinfo(CODESET) returned an empty string");
	pc_pass();
}
