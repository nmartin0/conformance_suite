/*
 * An incomplete character at the end of the input stops conversion with EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iconv.005
 */
#include "iv.h"

int main(void)
{
	/* the first byte of a two byte character, with nothing after it */
	char in[] = { 'a', 'b', (char)0xC3 };
	char out[16];
	iconv_t cd;
	struct iv_res r;

	pc_begin("XSH.iconv.005");
	cd = iv_open(iv_target, iv_utf8, NULL, NULL, NULL);
	iv_need(cd);
	pc_test();
	r = iv_call(cd, in, sizeof in, out, sizeof out);
	pc_check(r.ret == (size_t)-1, "an incomplete trailing character did not give -1");
	pc_check(r.err == EINVAL, "errno %s, expected EINVAL", pc_errname(r.err));
	pc_check(r.inp == in + 2 && r.inleft == 1, "input stopped at offset %ld with %lu left, expected 2 and 1",
	    (long)(r.inp - in), (unsigned long)r.inleft);
	pc_check(r.outp == out + 2 && memcmp(out, "ab", 2) == 0, "output is not the two complete characters");
	(void)iconv_close(cd);
	pc_pass();
}
