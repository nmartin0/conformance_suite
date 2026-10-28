/*
 * confstr() supports every name of its list that <unistd.h> defines.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.confstr.001
 * MUTANT    confstr.drops_v7
 * MUTANT    confstr.no_value_sets_errno
 * VARIANT   confstr.no_value
 * VARIANT   confstr.errno_on_success
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define SENTINEL 12345	/* an errno value no function sets */

/* The names of the confstr() list. */
static const struct {
	const char *name;
	int cs;
} vars[] = {
#ifdef _CS_PATH
	{ "_CS_PATH", _CS_PATH },
#endif
#ifdef _CS_POSIX_V8_ILP32_OFF32_CFLAGS
	{ "_CS_POSIX_V8_ILP32_OFF32_CFLAGS", _CS_POSIX_V8_ILP32_OFF32_CFLAGS },
#endif
#ifdef _CS_POSIX_V8_ILP32_OFF32_LDFLAGS
	{ "_CS_POSIX_V8_ILP32_OFF32_LDFLAGS", _CS_POSIX_V8_ILP32_OFF32_LDFLAGS },
#endif
#ifdef _CS_POSIX_V8_ILP32_OFF32_LIBS
	{ "_CS_POSIX_V8_ILP32_OFF32_LIBS", _CS_POSIX_V8_ILP32_OFF32_LIBS },
#endif
#ifdef _CS_POSIX_V8_ILP32_OFFBIG_CFLAGS
	{ "_CS_POSIX_V8_ILP32_OFFBIG_CFLAGS", _CS_POSIX_V8_ILP32_OFFBIG_CFLAGS },
#endif
#ifdef _CS_POSIX_V8_ILP32_OFFBIG_LDFLAGS
	{ "_CS_POSIX_V8_ILP32_OFFBIG_LDFLAGS", _CS_POSIX_V8_ILP32_OFFBIG_LDFLAGS },
#endif
#ifdef _CS_POSIX_V8_ILP32_OFFBIG_LIBS
	{ "_CS_POSIX_V8_ILP32_OFFBIG_LIBS", _CS_POSIX_V8_ILP32_OFFBIG_LIBS },
#endif
#ifdef _CS_POSIX_V8_LP64_OFF64_CFLAGS
	{ "_CS_POSIX_V8_LP64_OFF64_CFLAGS", _CS_POSIX_V8_LP64_OFF64_CFLAGS },
#endif
#ifdef _CS_POSIX_V8_LP64_OFF64_LDFLAGS
	{ "_CS_POSIX_V8_LP64_OFF64_LDFLAGS", _CS_POSIX_V8_LP64_OFF64_LDFLAGS },
#endif
#ifdef _CS_POSIX_V8_LP64_OFF64_LIBS
	{ "_CS_POSIX_V8_LP64_OFF64_LIBS", _CS_POSIX_V8_LP64_OFF64_LIBS },
#endif
#ifdef _CS_POSIX_V8_LPBIG_OFFBIG_CFLAGS
	{ "_CS_POSIX_V8_LPBIG_OFFBIG_CFLAGS", _CS_POSIX_V8_LPBIG_OFFBIG_CFLAGS },
#endif
#ifdef _CS_POSIX_V8_LPBIG_OFFBIG_LDFLAGS
	{ "_CS_POSIX_V8_LPBIG_OFFBIG_LDFLAGS", _CS_POSIX_V8_LPBIG_OFFBIG_LDFLAGS },
#endif
#ifdef _CS_POSIX_V8_LPBIG_OFFBIG_LIBS
	{ "_CS_POSIX_V8_LPBIG_OFFBIG_LIBS", _CS_POSIX_V8_LPBIG_OFFBIG_LIBS },
#endif
#ifdef _CS_POSIX_V8_THREADS_CFLAGS
	{ "_CS_POSIX_V8_THREADS_CFLAGS", _CS_POSIX_V8_THREADS_CFLAGS },
#endif
#ifdef _CS_POSIX_V8_THREADS_LDFLAGS
	{ "_CS_POSIX_V8_THREADS_LDFLAGS", _CS_POSIX_V8_THREADS_LDFLAGS },
#endif
#ifdef _CS_POSIX_V8_WIDTH_RESTRICTED_ENVS
	{ "_CS_POSIX_V8_WIDTH_RESTRICTED_ENVS", _CS_POSIX_V8_WIDTH_RESTRICTED_ENVS },
#endif
#ifdef _CS_V8_ENV
	{ "_CS_V8_ENV", _CS_V8_ENV },
#endif
#ifdef _CS_POSIX_V7_ILP32_OFF32_CFLAGS
	{ "_CS_POSIX_V7_ILP32_OFF32_CFLAGS", _CS_POSIX_V7_ILP32_OFF32_CFLAGS },
#endif
#ifdef _CS_POSIX_V7_ILP32_OFF32_LDFLAGS
	{ "_CS_POSIX_V7_ILP32_OFF32_LDFLAGS", _CS_POSIX_V7_ILP32_OFF32_LDFLAGS },
#endif
#ifdef _CS_POSIX_V7_ILP32_OFF32_LIBS
	{ "_CS_POSIX_V7_ILP32_OFF32_LIBS", _CS_POSIX_V7_ILP32_OFF32_LIBS },
#endif
#ifdef _CS_POSIX_V7_ILP32_OFFBIG_CFLAGS
	{ "_CS_POSIX_V7_ILP32_OFFBIG_CFLAGS", _CS_POSIX_V7_ILP32_OFFBIG_CFLAGS },
#endif
#ifdef _CS_POSIX_V7_ILP32_OFFBIG_LDFLAGS
	{ "_CS_POSIX_V7_ILP32_OFFBIG_LDFLAGS", _CS_POSIX_V7_ILP32_OFFBIG_LDFLAGS },
#endif
#ifdef _CS_POSIX_V7_ILP32_OFFBIG_LIBS
	{ "_CS_POSIX_V7_ILP32_OFFBIG_LIBS", _CS_POSIX_V7_ILP32_OFFBIG_LIBS },
#endif
#ifdef _CS_POSIX_V7_LP64_OFF64_CFLAGS
	{ "_CS_POSIX_V7_LP64_OFF64_CFLAGS", _CS_POSIX_V7_LP64_OFF64_CFLAGS },
#endif
#ifdef _CS_POSIX_V7_LP64_OFF64_LDFLAGS
	{ "_CS_POSIX_V7_LP64_OFF64_LDFLAGS", _CS_POSIX_V7_LP64_OFF64_LDFLAGS },
#endif
#ifdef _CS_POSIX_V7_LP64_OFF64_LIBS
	{ "_CS_POSIX_V7_LP64_OFF64_LIBS", _CS_POSIX_V7_LP64_OFF64_LIBS },
#endif
#ifdef _CS_POSIX_V7_LPBIG_OFFBIG_CFLAGS
	{ "_CS_POSIX_V7_LPBIG_OFFBIG_CFLAGS", _CS_POSIX_V7_LPBIG_OFFBIG_CFLAGS },
#endif
#ifdef _CS_POSIX_V7_LPBIG_OFFBIG_LDFLAGS
	{ "_CS_POSIX_V7_LPBIG_OFFBIG_LDFLAGS", _CS_POSIX_V7_LPBIG_OFFBIG_LDFLAGS },
#endif
#ifdef _CS_POSIX_V7_LPBIG_OFFBIG_LIBS
	{ "_CS_POSIX_V7_LPBIG_OFFBIG_LIBS", _CS_POSIX_V7_LPBIG_OFFBIG_LIBS },
#endif
#ifdef _CS_POSIX_V7_THREADS_CFLAGS
	{ "_CS_POSIX_V7_THREADS_CFLAGS", _CS_POSIX_V7_THREADS_CFLAGS },
#endif
#ifdef _CS_POSIX_V7_THREADS_LDFLAGS
	{ "_CS_POSIX_V7_THREADS_LDFLAGS", _CS_POSIX_V7_THREADS_LDFLAGS },
#endif
#ifdef _CS_POSIX_V7_WIDTH_RESTRICTED_ENVS
	{ "_CS_POSIX_V7_WIDTH_RESTRICTED_ENVS", _CS_POSIX_V7_WIDTH_RESTRICTED_ENVS },
#endif
#ifdef _CS_V7_ENV
	{ "_CS_V7_ENV", _CS_V7_ENV },
#endif
};

/* Names of the list <unistd.h> does not define. */
static const char *const missing[] = {
#ifndef _CS_PATH
	"_CS_PATH",
#endif
#ifndef _CS_POSIX_V8_ILP32_OFF32_CFLAGS
	"_CS_POSIX_V8_ILP32_OFF32_CFLAGS",
#endif
#ifndef _CS_POSIX_V8_ILP32_OFF32_LDFLAGS
	"_CS_POSIX_V8_ILP32_OFF32_LDFLAGS",
#endif
#ifndef _CS_POSIX_V8_ILP32_OFF32_LIBS
	"_CS_POSIX_V8_ILP32_OFF32_LIBS",
#endif
#ifndef _CS_POSIX_V8_ILP32_OFFBIG_CFLAGS
	"_CS_POSIX_V8_ILP32_OFFBIG_CFLAGS",
#endif
#ifndef _CS_POSIX_V8_ILP32_OFFBIG_LDFLAGS
	"_CS_POSIX_V8_ILP32_OFFBIG_LDFLAGS",
#endif
#ifndef _CS_POSIX_V8_ILP32_OFFBIG_LIBS
	"_CS_POSIX_V8_ILP32_OFFBIG_LIBS",
#endif
#ifndef _CS_POSIX_V8_LP64_OFF64_CFLAGS
	"_CS_POSIX_V8_LP64_OFF64_CFLAGS",
#endif
#ifndef _CS_POSIX_V8_LP64_OFF64_LDFLAGS
	"_CS_POSIX_V8_LP64_OFF64_LDFLAGS",
#endif
#ifndef _CS_POSIX_V8_LP64_OFF64_LIBS
	"_CS_POSIX_V8_LP64_OFF64_LIBS",
#endif
#ifndef _CS_POSIX_V8_LPBIG_OFFBIG_CFLAGS
	"_CS_POSIX_V8_LPBIG_OFFBIG_CFLAGS",
#endif
#ifndef _CS_POSIX_V8_LPBIG_OFFBIG_LDFLAGS
	"_CS_POSIX_V8_LPBIG_OFFBIG_LDFLAGS",
#endif
#ifndef _CS_POSIX_V8_LPBIG_OFFBIG_LIBS
	"_CS_POSIX_V8_LPBIG_OFFBIG_LIBS",
#endif
#ifndef _CS_POSIX_V8_THREADS_CFLAGS
	"_CS_POSIX_V8_THREADS_CFLAGS",
#endif
#ifndef _CS_POSIX_V8_THREADS_LDFLAGS
	"_CS_POSIX_V8_THREADS_LDFLAGS",
#endif
#ifndef _CS_POSIX_V8_WIDTH_RESTRICTED_ENVS
	"_CS_POSIX_V8_WIDTH_RESTRICTED_ENVS",
#endif
#ifndef _CS_V8_ENV
	"_CS_V8_ENV",
#endif
#ifndef _CS_POSIX_V7_ILP32_OFF32_CFLAGS
	"_CS_POSIX_V7_ILP32_OFF32_CFLAGS",
#endif
#ifndef _CS_POSIX_V7_ILP32_OFF32_LDFLAGS
	"_CS_POSIX_V7_ILP32_OFF32_LDFLAGS",
#endif
#ifndef _CS_POSIX_V7_ILP32_OFF32_LIBS
	"_CS_POSIX_V7_ILP32_OFF32_LIBS",
#endif
#ifndef _CS_POSIX_V7_ILP32_OFFBIG_CFLAGS
	"_CS_POSIX_V7_ILP32_OFFBIG_CFLAGS",
#endif
#ifndef _CS_POSIX_V7_ILP32_OFFBIG_LDFLAGS
	"_CS_POSIX_V7_ILP32_OFFBIG_LDFLAGS",
#endif
#ifndef _CS_POSIX_V7_ILP32_OFFBIG_LIBS
	"_CS_POSIX_V7_ILP32_OFFBIG_LIBS",
#endif
#ifndef _CS_POSIX_V7_LP64_OFF64_CFLAGS
	"_CS_POSIX_V7_LP64_OFF64_CFLAGS",
#endif
#ifndef _CS_POSIX_V7_LP64_OFF64_LDFLAGS
	"_CS_POSIX_V7_LP64_OFF64_LDFLAGS",
#endif
#ifndef _CS_POSIX_V7_LP64_OFF64_LIBS
	"_CS_POSIX_V7_LP64_OFF64_LIBS",
#endif
#ifndef _CS_POSIX_V7_LPBIG_OFFBIG_CFLAGS
	"_CS_POSIX_V7_LPBIG_OFFBIG_CFLAGS",
#endif
#ifndef _CS_POSIX_V7_LPBIG_OFFBIG_LDFLAGS
	"_CS_POSIX_V7_LPBIG_OFFBIG_LDFLAGS",
#endif
#ifndef _CS_POSIX_V7_LPBIG_OFFBIG_LIBS
	"_CS_POSIX_V7_LPBIG_OFFBIG_LIBS",
#endif
#ifndef _CS_POSIX_V7_THREADS_CFLAGS
	"_CS_POSIX_V7_THREADS_CFLAGS",
#endif
#ifndef _CS_POSIX_V7_THREADS_LDFLAGS
	"_CS_POSIX_V7_THREADS_LDFLAGS",
#endif
#ifndef _CS_POSIX_V7_WIDTH_RESTRICTED_ENVS
	"_CS_POSIX_V7_WIDTH_RESTRICTED_ENVS",
#endif
#ifndef _CS_V7_ENV
	"_CS_V7_ENV",
#endif
	NULL
};

/* Notes the names <unistd.h> lacks (a header matter, XBD <unistd.h>). */
static void note_missing(void)
{
	size_t i;

	for (i = 0; missing[i] != NULL; i++)
		pc_note("<unistd.h> does not define %s; not checked here", missing[i]);
}

int main(void)
{
	size_t i, j, n, m;
	int e;
	char buf[64];

	pc_begin("XSH.confstr.001");
	note_missing();
	pc_test();
	for (i = 0; i < sizeof vars / sizeof vars[0]; i++) {
		errno = SENTINEL;
		n = confstr(vars[i].cs, NULL, 0);
		e = errno;
		pc_check(n > 0 || e == SENTINEL, "confstr(%s, NULL, 0) returned 0 with errno %s: the name is not supported",
			 vars[i].name, pc_errname(e));
		if (n == 0)
			continue;
		/* A value was reported: the size it names is that of the string the call then stores (cut to the buffer, with a terminator). */
		for (j = 0; j < sizeof buf; j++)
			buf[j] = 'x';
		m = confstr(vars[i].cs, buf, sizeof buf);
		pc_check(m == n, "confstr(%s, buf, %zu) returned %zu, but confstr(%s, NULL, 0) returned %zu",
			 vars[i].name, sizeof buf, m, vars[i].name, n);
		if (m == n && n <= sizeof buf) {
			for (j = 0; j < sizeof buf && buf[j] != '\0'; j++)
				;
			pc_check(j == n - 1, "confstr(%s): the string stored is not %zu bytes with its terminator", vars[i].name, n);
		}
		else if (m == n)
			pc_check(buf[sizeof buf - 1] == '\0', "confstr(%s): the truncated string is not terminated", vars[i].name);
	}
	pc_pass();
}
