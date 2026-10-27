/*
 * Closing a conversion descriptor releases any file descriptor used for it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iconv_close.002
 */
#include "iv.h"

int main(void)
{
	static unsigned char before[IV_FDS], after[IV_FDS];
	const char *to = NULL, *from = NULL;
	iconv_t cd;
	int fd;

	pc_begin("XSH.iconv_close.002");
	cd = iv_open(iv_all, iv_all, NULL, &to, &from);
	iv_need(cd);
	(void)iconv_close(cd);
	pc_test();
	iv_fdmap(before);
	cd = iconv_open(to, from);
	pc_check(cd != IV_BAD, "iconv_open failed the second time: %s", pc_errname(errno));
	pc_check(iconv_close(cd) == 0, "iconv_close failed");
	iv_fdmap(after);
	for (fd = 0; fd < IV_FDS; fd++)
		pc_check(before[fd] == after[fd], "descriptor %d is %s after iconv_open and iconv_close",
		    fd, after[fd] ? "open although it was closed before" : "closed although it was open before");
	pc_pass();
}
