/*
 * strsignal.c - conforming variants of strsignal(), for the variant
 * checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * "own_text": this system's descriptions are its own wording, "signal
 * number <n>", used alike by strsignal(), psignal() and psiginfo().
 * strsignal() DESCRIPTION leaves the string implementation-defined and
 * asks only that it be psignal()'s set of messages; psiginfo() DESCRIPTION
 * fixes the message, colon, space and newline around it, not its words.
 * Each signal has its own string, so no call overwrites another's.
 * "static_buf": strsignal() copies its description into one buffer that
 * the next strsignal() call overwrites. DESCRIPTION allows a later
 * strsignal() call to overwrite the string (and only that, or
 * setlocale()).
 */
static char own[256][32];
static char one[256];

char *strsignal(int sig)
{
	const char *v = variant_of("strsignal.");
	REAL(strsignal);
	char *s = real_strsignal(sig);

	if (IS("own_text") && sig > 0 && sig < 256) {
		(void)snprintf(own[sig], sizeof own[sig], "signal number %d", sig);
		return own[sig];
	}
	if (IS("static_buf") && s != NULL) {
		(void)snprintf(one, sizeof one, "%s", s);
		return one;
	}
	return s;
}

/* 1 if the line went out; errno is kept on success, as psignal() requires. */
static int own_line(int sig, const char *msg)
{
	int e = errno, r;

	if (msg != NULL && *msg != '\0')
		r = fprintf(stderr, "%s: signal number %d\n", msg, sig);
	else
		r = fprintf(stderr, "signal number %d\n", sig);
	if (r < 0)
		return 0;
	errno = e;
	return 1;
}

void psignal(int sig, const char *msg)
{
	const char *v = variant_of("strsignal.");
	REAL(psignal);

	if (IS("own_text") && sig > 0 && sig < 256 && own_line(sig, msg))
		return;
	real_psignal(sig, msg);
}

void psiginfo(const siginfo_t *si, const char *msg)
{
	const char *v = variant_of("strsignal.");
	REAL(psiginfo);

	if (IS("own_text") && si->si_signo > 0 && si->si_signo < 256 && own_line(si->si_signo, msg))
		return;
	real_psiginfo(si, msg);
}
