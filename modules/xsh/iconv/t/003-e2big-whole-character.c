/*
 * Conversion stops before an input character whose output would not fit.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iconv.003
 */
#include "iv.h"

int main(void)
{
	/* 'a', the character U+00E9 as one ISO 8859-1 byte, 'b' */
	char in[] = { 'a', (char)0xE9, 'b' };
	char out[2];
	iconv_t cd;
	struct iv_res r;

	pc_begin("XSH.iconv.003");
	cd = iv_open(iv_utf8, iv_latin1, NULL, NULL, NULL);
	iv_need(cd);
	pc_test();
	r = iv_call(cd, in, sizeof in, out, sizeof out);
	pc_check(r.ret == (size_t)-1, "no failure although U+00E9 needs two output bytes and one is left");
	pc_check(r.err == E2BIG, "errno %s, expected E2BIG", pc_errname(r.err));
	pc_check(r.inp == in + 1 && r.inleft == 2, "input stopped at offset %ld, expected 1 (just before the character)",
	    (long)(r.inp - in));
	pc_check(r.outp == out + 1 && r.outleft == 1 && out[0] == 'a', "output holds more or less than the first character");
	(void)iconv_close(cd);
	pc_pass();
}
