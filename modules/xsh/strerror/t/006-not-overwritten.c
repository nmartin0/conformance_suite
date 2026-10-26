/*
 * Other functions do not overwrite the string strerror() returned.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strerror.006
 * MUTANT    strerror.perror_overwrites
 * MUTANT    strerror.r_shares_buffer
 * VARIANT   strerror.global_buf
 * VARIANT   strerror.r_errno
 */
#include "pc.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
	static char copy[4096], buf[4096];
	const char *s;

	pc_begin("XSH.strerror.006");
	pc_test();
	s = strerror(ENOENT);
	pc_check(s != NULL, "strerror(ENOENT) returned a null pointer");
	if (strlen(s) >= sizeof copy)
		pc_unresolved("the message is longer than %zu bytes", sizeof copy - 1);
	strcpy(copy, s);
	errno = EACCES;
	perror("pc");
	(void)strerror_r(EINVAL, buf, sizeof buf);
	(void)strsignal(SIGINT);
	psignal(SIGTERM, "pc");
	pc_check(strcmp(s, copy) == 0, "the strerror(ENOENT) string changed from \"%s\" to \"%s\"", copy, s);
	pc_pass();
}
