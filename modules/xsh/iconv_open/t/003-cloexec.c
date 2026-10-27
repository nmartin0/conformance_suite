/*
 * A descriptor that iconv_open() creates is close-on-exec.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.iconv_open.003
 */
#include "iv.h"

int main(void)
{
	static unsigned char before[IV_FDS], after[IV_FDS];
	const char *to = NULL, *from = NULL;
	iconv_t cd;
	int fd, flags;

	pc_begin("XSH.iconv_open.003");
	cd = iv_open(iv_all, iv_all, NULL, &to, &from);
	iv_need(cd);
	(void)iconv_close(cd);
	pc_test();
	iv_fdmap(before);
	cd = iconv_open(to, from);
	pc_check(cd != IV_BAD, "iconv_open failed the second time: %s", pc_errname(errno));
	iv_fdmap(after);
	for (fd = 0; fd < IV_FDS; fd++) {
		if (after[fd] && !before[fd]) {
			flags = fcntl(fd, F_GETFD);
			pc_check(flags != -1 && (flags & FD_CLOEXEC) != 0,
			    "descriptor %d opened by iconv_open() lacks FD_CLOEXEC", fd);
		}
	}
	(void)iconv_close(cd);
	pc_pass();
}
