/*
 * confstr() returns the size the whole value needs, terminating null
 * included, with and without a buffer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.confstr.002
 * MUTANT    confstr.length_without_null
 * MUTANT    confstr.null_buffer_zero
 * VARIANT   confstr.no_value
 * VARIANT   confstr.errno_on_success
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
	static char buf[65536];
	size_t i, n, m;
	int checked = 0;

	pc_begin("XSH.confstr.002");
	note_missing();
	pc_test();
	for (i = 0; i < sizeof vars / sizeof vars[0]; i++) {
		memset(buf, 'x', sizeof buf);
		n = confstr(vars[i].cs, buf, sizeof buf);
		if (n == 0)
			continue;	/* no configuration-defined value */
		if (n > sizeof buf)
			pc_unresolved("the value of %s needs %zu bytes", vars[i].name, n);
		checked++;
		pc_check(memchr(buf, '\0', sizeof buf) != NULL && strlen(buf) == n - 1,
			 "confstr(%s) returned %zu but stored a string of %zu bytes", vars[i].name, n,
			 memchr(buf, '\0', sizeof buf) != NULL ? strlen(buf) : sizeof buf);
		m = confstr(vars[i].cs, NULL, 0);
		pc_check(m == n, "confstr(%s, NULL, 0) returned %zu, but %zu with a buffer", vars[i].name, m, n);
	}
	pc_check(checked > 0, "no name of the list has a value; _CS_PATH must have one (XSH.confstr.005)");
	pc_pass();
}
