/*
 * psiginfo.c - conforming variants of psiginfo() and psignal(), for the
 * variant checks (CI only).
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
 * "own_text": the description is this system's own wording ("signal
 * number <n>"), and the line goes out in pieces, one write() each, with
 * the stream left alone. psiginfo() DESCRIPTION only asks for a
 * language-dependent description of the signal after the optional
 * message, colon and space, then a newline, with errno kept on success
 * and the orientation of stderr unchanged; it does not fix the wording or
 * how the bytes reach the file. On a write error the system's own
 * function is used, which sets the error indicator and errno as
 * DESCRIPTION requires (the same write fails for it).
 */
static int put(int fd, const char *s)
{
	size_t n = strlen(s), done = 0;
	ssize_t k;

	while (done < n) {
		k = write(fd, s + done, n - done);
		if (k < 0) {
			if (errno == EINTR)
				continue;
			return 0;
		}
		done += (size_t)k;
	}
	return 1;
}

/* 1 if the line was written; errno is kept on success. */
static int own_line(int sig, const char *message)
{
	char desc[64];
	int e = errno, fd, ok = 1;

	(void)fflush(stderr);
	fd = fileno(stderr);
	(void)snprintf(desc, sizeof desc, "signal number %d", sig);
	if (message != NULL && *message != '\0')
		ok = put(fd, message) && put(fd, ": ");
	ok = ok && put(fd, desc) && put(fd, "\n");
	if (ok)
		errno = e;
	return ok;
}

void psignal(int sig, const char *message)
{
	const char *v = variant_of("psiginfo.");
	REAL(psignal);

	if (IS("own_text") && own_line(sig, message))
		return;
	real_psignal(sig, message);
}

void psiginfo(const siginfo_t *pinfo, const char *message)
{
	const char *v = variant_of("psiginfo.");
	REAL(psiginfo);

	if (IS("own_text") && own_line(pinfo->si_signo, message))
		return;
	real_psiginfo(pinfo, message);
}
