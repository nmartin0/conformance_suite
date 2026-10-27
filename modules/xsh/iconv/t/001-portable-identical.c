/*
 * Portable text converts to identical bytes; every pointer and count is updated.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iconv.001
 */
#include "iv.h"

int main(void)
{
	char in[] = "Hello, world 0123";
	char out[64];
	size_t n = sizeof in - 1;
	iconv_t cd;
	struct iv_res r;

	pc_begin("XSH.iconv.001");
	cd = iv_open(iv_all, iv_all, NULL, NULL, NULL);
	iv_need(cd);
	pc_test();
	r = iv_call(cd, in, n, out, sizeof out);
	pc_check(r.ret == 0, "return value %lu (errno %s), expected 0 for text that converts identically",
	    (unsigned long)r.ret, pc_errname(r.err));
	pc_check(r.inleft == 0, "inbytesleft is %lu, expected 0", (unsigned long)r.inleft);
	pc_check(r.inp == in + n, "inbuf not moved past the converted input");
	pc_check(r.outp == out + n, "outbuf advanced by %ld, expected %lu", (long)(r.outp - out), (unsigned long)n);
	pc_check(r.outleft == sizeof out - n, "outbytesleft is %lu, expected %lu",
	    (unsigned long)r.outleft, (unsigned long)(sizeof out - n));
	pc_check(memcmp(out, in, n) == 0, "converted bytes differ from the input");
	pc_check(iconv_close(cd) == 0, "iconv_close failed");
	pc_pass();
}
