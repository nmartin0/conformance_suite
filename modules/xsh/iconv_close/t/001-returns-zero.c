/*
 * iconv_close() of an open descriptor returns 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iconv_close.001
 */
#include "iv.h"

int main(void)
{
	iconv_t cd;
	int r;

	pc_begin("XSH.iconv_close.001");
	cd = iv_open(iv_all, iv_all, NULL, NULL, NULL);
	iv_need(cd);
	pc_test();
	errno = 0;
	r = iconv_close(cd);
	pc_check(r == 0, "iconv_close returned %d (errno %s), expected 0", r, pc_errname(errno));
	pc_pass();
}
