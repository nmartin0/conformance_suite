/*
 * A zero byte inside the input is converted like any other character.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iconv.012
 */
#include "iv.h"

int main(void)
{
	char in[] = { 'a', '\0', 'b' };
	char out[16];
	iconv_t cd;
	struct iv_res r;

	pc_begin("XSH.iconv.012");
	cd = iv_open(iv_all, iv_all, NULL, NULL, NULL);
	iv_need(cd);
	pc_test();
	r = iv_call(cd, in, sizeof in, out, sizeof out);
	pc_check(r.ret == 0, "return value %lu (errno %s), expected 0", (unsigned long)r.ret, pc_errname(r.err));
	pc_check(r.inleft == 0, "input left %lu: the zero byte ended the conversion", (unsigned long)r.inleft);
	pc_check(r.outp == out + 3 && out[0] == 'a' && out[1] == '\0' && out[2] == 'b', "output is not a, NUL, b");
	(void)iconv_close(cd);
	pc_pass();
}
