/*
 * cg.h - helpers for the message catalog tests: build a catalog with the
 * gencat utility, and keep track of open descriptors.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * The catalog format is unspecified, so every catalog the tests open is
 * made by gencat from a message source file; if gencat cannot be run the
 * test is UNRESOLVED.
 */
#ifndef CG_H
#define CG_H

#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <nl_types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CG_BAD ((nl_catd)-1)

/* Messages used by most tests: two sets, ids that are not contiguous. */
#define CG_SRC \
	"$ test catalog\n" \
	"$set 1 first set\n" \
	"1 Hello catalog\n" \
	"2 Second line\n" \
	"$set 4 other set\n" \
	"7 Seven\n" \
	"20 Twenty\n"

/* Write the source text to msgfile and run gencat to make catfile. */
static inline void cg_make(const char *catfile, const char *msgfile, const char *src)
{
	char cmd[600];

	pc_make_file(msgfile, src);
	(void)snprintf(cmd, sizeof cmd, "gencat '%s' '%s' >/dev/null 2>&1", catfile, msgfile);
	if (system(cmd) != 0)
		pc_unresolved("the gencat utility could not build the test catalog");
}

#define CG_FDS 1024

/* Which descriptors below CG_FDS are open. */
static inline void cg_fdmap(unsigned char *m)
{
	int fd;

	for (fd = 0; fd < CG_FDS; fd++)
		m[fd] = fcntl(fd, F_GETFD) != -1;
}

/*
 * NLSPATH in the environment of a process with appropriate privileges
 * makes catopen() undefined (it matters only if NLSPATH existed when the
 * process started). Call right after pc_begin(), before any setenv().
 */
static inline void cg_nlspath_guard(void)
{
	if (getenv("NLSPATH") != NULL && geteuid() == 0)
		pc_unresolved("NLSPATH was in the environment at start and the process is privileged: catopen() is undefined");
}

static inline void cg_setpath(const char *v)
{
	if (setenv("NLSPATH", v, 1) != 0)
		pc_unresolved("setenv NLSPATH failed");
}

#endif
