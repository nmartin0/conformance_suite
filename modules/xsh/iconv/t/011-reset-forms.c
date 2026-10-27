/*
 * The reset calls (null inbuf, or inbuf pointing to a null pointer) are not errors.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iconv.011
 */
#include "iv.h"

int main(void)
{
	char in[] = "abc";
	char out[64], out2[16];
	char *np = NULL, *op;
	size_t il = 0, ol;
	iconv_t cd;
	struct iv_res r;

	pc_begin("XSH.iconv.011");
	cd = iv_open(iv_all, iv_all, NULL, NULL, NULL);
	iv_need(cd);
	pc_test();
	r = iv_call(cd, in, 3, out, sizeof out);
	pc_check(r.ret != (size_t)-1, "plain conversion failed: %s", pc_errname(r.err));
	/* null inbuf, null outbuf */
	errno = 0;
	pc_check(iconv(cd, NULL, NULL, NULL, NULL) != (size_t)-1, "iconv with a null inbuf and null outbuf failed: %s",
	    pc_errname(errno));
	/* null inbuf with an output buffer */
	op = out2;
	ol = sizeof out2;
	errno = 0;
	pc_check(iconv(cd, NULL, NULL, &op, &ol) != (size_t)-1, "iconv with a null inbuf and an output buffer failed: %s",
	    pc_errname(errno));
	pc_check(op >= out2 && (size_t)(op - out2) <= sizeof out2 && ol == sizeof out2 - (size_t)(op - out2),
	    "reset call left outbuf and outbytesleft inconsistent");
	/* inbuf pointing to a null pointer */
	op = out2;
	ol = sizeof out2;
	errno = 0;
	pc_check(iconv(cd, &np, &il, &op, &ol) != (size_t)-1, "iconv with inbuf pointing to a null pointer failed: %s",
	    pc_errname(errno));
	/* the descriptor still works */
	in[0] = 'x';
	in[1] = 'y';
	in[2] = 'z';
	r = iv_call(cd, in, 3, out, sizeof out);
	pc_check(r.ret != (size_t)-1 && r.inleft == 0 && r.outp == out + 3 && memcmp(out, "xyz", 3) == 0,
	    "conversion after a reset failed");
	(void)iconv_close(cd);
	pc_pass();
}
