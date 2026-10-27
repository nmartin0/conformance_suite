/*
 * The three indicator suffixes can be appended to the target name.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iconv_open.002
 */
#include "iv.h"

int main(void)
{
	static const char *const suf[] = { "//IGNORE", "//NON_IDENTICAL_DISCARD", "//TRANSLIT" };
	const char *to = NULL, *from = NULL;
	char name[96];
	iconv_t cd, base;
	size_t i;

	pc_begin("XSH.iconv_open.002");
	base = iv_open(iv_all, iv_all, NULL, &to, &from);
	iv_need(base);
	(void)iconv_close(base);
	pc_test();
	for (i = 0; i < sizeof suf / sizeof suf[0]; i++) {
		(void)snprintf(name, sizeof name, "%s%s", to, suf[i]);
		errno = 0;
		cd = iconv_open(name, from);
		pc_check(cd != IV_BAD, "iconv_open(\"%s\", \"%s\") failed (%s) although \"%s\" to \"%s\" opens",
		    name, from, pc_errname(errno), to, from);
		(void)iconv_close(cd);
	}
	pc_pass();
}
