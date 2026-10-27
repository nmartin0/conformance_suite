/*
 * A byte outside the input code set stops conversion with EILSEQ.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iconv.004
 */
#include "iv.h"

int main(void)
{
	char in[] = { 'a', 'b', (char)0xFF, 'c', 'd' };
	char out[16];
	iconv_t cd;
	struct iv_res r;

	pc_begin("XSH.iconv.004");
	cd = iv_open(iv_target, iv_utf8, NULL, NULL, NULL);
	iv_need(cd);
	pc_test();
	r = iv_call(cd, in, sizeof in, out, sizeof out);
	pc_check(r.ret == (size_t)-1, "an invalid input byte did not give -1");
	pc_check(r.err == EILSEQ, "errno %s, expected EILSEQ", pc_errname(r.err));
	pc_check(r.inp == in + 2 && r.inleft == 3, "input stopped at offset %ld with %lu left, expected 2 and 3",
	    (long)(r.inp - in), (unsigned long)r.inleft);
	pc_check(r.outp == out + 2 && memcmp(out, "ab", 2) == 0, "output is not the two characters before the bad byte");
	(void)iconv_close(cd);
	pc_pass();
}
