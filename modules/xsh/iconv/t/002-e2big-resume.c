/*
 * A full output buffer stops conversion with E2BIG; the next call resumes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iconv.002
 */
#include "iv.h"

int main(void)
{
	char in[] = "abcdef";
	char small[3], rest[16], exact[6];
	iconv_t cd;
	struct iv_res r1, r2, r3;

	pc_begin("XSH.iconv.002");
	cd = iv_open(iv_all, iv_all, NULL, NULL, NULL);
	iv_need(cd);
	pc_test();
	r1 = iv_call(cd, in, 6, small, sizeof small);
	pc_check(r1.ret == (size_t)-1, "a 3 byte output buffer for 6 bytes of text did not give -1");
	pc_check(r1.err == E2BIG, "errno %s, expected E2BIG", pc_errname(r1.err));
	pc_check(r1.inleft == 3 && r1.inp == in + 3, "input left %lu, expected 3 after stopping on a full buffer",
	    (unsigned long)r1.inleft);
	pc_check(r1.outleft == 0 && r1.outp == small + 3, "output not filled exactly");
	pc_check(memcmp(small, "abc", 3) == 0, "first part of the output is wrong");
	r2 = iv_call(cd, r1.inp, r1.inleft, rest, sizeof rest);
	pc_check(r2.ret != (size_t)-1, "resuming with a larger buffer failed: %s", pc_errname(r2.err));
	pc_check(r2.inleft == 0, "input left after resuming: %lu", (unsigned long)r2.inleft);
	pc_check(r2.outp == rest + 3 && memcmp(rest, "def", 3) == 0, "second part of the output is wrong");
	/* a buffer of exactly the needed size is enough */
	r3 = iv_call(cd, in, 6, exact, sizeof exact);
	pc_check(r3.ret != (size_t)-1, "an output buffer of exactly the needed size failed: %s", pc_errname(r3.err));
	pc_check(r3.inleft == 0 && r3.outleft == 0, "exact fit not consumed completely");
	pc_check(memcmp(exact, in, 6) == 0, "exact fit output wrong");
	(void)iconv_close(cd);
	pc_pass();
}
