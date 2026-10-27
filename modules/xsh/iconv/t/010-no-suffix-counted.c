/*
 * Without a suffix a character with no identical counterpart is converted some way and counted.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iconv.010
 */
#include "iv.h"

int main(void)
{
	char in[] = { 'a', (char)0xC3, (char)0xA9, 'b' };
	char out[32];
	iconv_t cd;
	struct iv_res r;

	pc_begin("XSH.iconv.010");
	cd = iv_open(iv_ascii, iv_utf8, NULL, NULL, NULL);
	iv_need(cd);
	pc_test();
	r = iv_call(cd, in, sizeof in, out, sizeof out);
	pc_check(r.ret != (size_t)-1, "a valid character without an identical counterpart gave an error (%s); "
	    "the text asks for an implementation-defined conversion", pc_errname(r.err));
	pc_check(r.ret == 1, "return value %lu, expected 1", (unsigned long)r.ret);
	pc_check(r.inleft == 0, "input left %lu, expected 0", (unsigned long)r.inleft);
	pc_check(r.outp - out >= 2 && out[0] == 'a' && r.outp[-1] == 'b',
	    "output does not start with 'a' and end with 'b'");
	(void)iconv_close(cd);
	pc_pass();
}
