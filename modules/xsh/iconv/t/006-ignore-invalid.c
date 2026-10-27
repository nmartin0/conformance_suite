/*
 * With //IGNORE an invalid input byte followed by a valid character is discarded.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iconv.006
 */
#include "iv.h"

int main(void)
{
	char in[] = { 'a', 'b', (char)0xFF, 'c', 'd' };
	char out[16];
	iconv_t cd;
	struct iv_res r;

	pc_begin("XSH.iconv.006");
	cd = iv_open(iv_ascii, iv_utf8, "//IGNORE", NULL, NULL);
	iv_need(cd);
	pc_test();
	r = iv_call(cd, in, sizeof in, out, sizeof out);
	pc_check(r.ret != (size_t)-1, "discarding an invalid byte was treated as an error: %s", pc_errname(r.err));
	pc_check(r.inleft == 0, "input left %lu, expected 0", (unsigned long)r.inleft);
	pc_check(r.outp == out + 4 && memcmp(out, "abcd", 4) == 0, "output is not \"abcd\"");
	(void)iconv_close(cd);
	pc_pass();
}
