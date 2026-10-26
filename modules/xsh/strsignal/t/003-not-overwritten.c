/*
 * Other functions do not overwrite the string strsignal() returned.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strsignal.003
 * MUTANT    strsignal.psignal_overwrites
 * MUTANT    strsignal.psiginfo_overwrites
 * VARIANT   strsignal.static_buf
 * VARIANT   strsignal.own_text
 */
#include "pc.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
	static char copy[1024];
	const char *s;
	siginfo_t si;

	pc_begin("XSH.strsignal.003");
	memset(&si, 0, sizeof si);
	si.si_signo = SIGUSR2;
	si.si_code = SI_USER;
	pc_test();
	s = strsignal(SIGINT);
	pc_check(s != NULL, "strsignal(SIGINT) returned a null pointer");
	if (strlen(s) >= sizeof copy)
		pc_unresolved("the string is longer than %zu bytes", sizeof copy - 1);
	strcpy(copy, s);
	psignal(SIGTERM, "pc");
	psiginfo(&si, "pc");
	(void)strerror(EINVAL);
	errno = ENOENT;
	perror("pc");
	pc_check(strcmp(s, copy) == 0, "the strsignal(SIGINT) string changed from \"%s\" to \"%s\"", copy, s);
	pc_pass();
}
