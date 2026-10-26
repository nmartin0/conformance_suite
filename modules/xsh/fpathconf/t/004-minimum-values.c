/*
 * pathconf() limits on a directory are -1 (no limit) or at least the
 * minimum acceptable values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fpathconf.004
 * MUTANT    fpathconf.path_max_255
 * MUTANT    fpathconf.pipe_buf_256
 * MUTANT    fpathconf.nolimit_sets_errno
 * VARIANT   fpathconf.no_limit
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* The names of the fpathconf() table. */
struct var {
	const char *name;
	int pc;
	char kind;	/* L a <limits.h> maximum or minimum, O an option, X a resolution */
	long min, xsi_min;	/* minimum acceptable values (-1: none) */
	const char *where;	/* file types it must apply to: D directory, R regular, F FIFO or pipe, T terminal */
	const char *dep;	/* option it depends on, or NULL */
};

static const struct var vars[] = {
#ifdef _PC_FILESIZEBITS
	{ "_PC_FILESIZEBITS", _PC_FILESIZEBITS, 'L', 32, 32, "D", NULL },
#endif
#ifdef _PC_LINK_MAX
	{ "_PC_LINK_MAX", _PC_LINK_MAX, 'L', 8, 8, "DR", NULL },
#endif
#ifdef _PC_MAX_CANON
	{ "_PC_MAX_CANON", _PC_MAX_CANON, 'L', 255, 255, "T", NULL },
#endif
#ifdef _PC_MAX_INPUT
	{ "_PC_MAX_INPUT", _PC_MAX_INPUT, 'L', 255, 255, "T", NULL },
#endif
#ifdef _PC_NAME_MAX
	{ "_PC_NAME_MAX", _PC_NAME_MAX, 'L', 14, 255, "D", NULL },
#endif
#ifdef _PC_PATH_MAX
	{ "_PC_PATH_MAX", _PC_PATH_MAX, 'L', 256, 1024, "D", NULL },
#endif
#ifdef _PC_PIPE_BUF
	{ "_PC_PIPE_BUF", _PC_PIPE_BUF, 'L', 512, 512, "DF", NULL },
#endif
#ifdef _PC_2_SYMLINKS
	{ "_PC_2_SYMLINKS", _PC_2_SYMLINKS, 'O', -1, -1, "D", NULL },
#endif
#ifdef _PC_ALLOC_SIZE_MIN
	{ "_PC_ALLOC_SIZE_MIN", _PC_ALLOC_SIZE_MIN, 'L', -1, -1, "R", "ADV" },
#endif
#ifdef _PC_REC_INCR_XFER_SIZE
	{ "_PC_REC_INCR_XFER_SIZE", _PC_REC_INCR_XFER_SIZE, 'L', -1, -1, "R", "ADV" },
#endif
#ifdef _PC_REC_MAX_XFER_SIZE
	{ "_PC_REC_MAX_XFER_SIZE", _PC_REC_MAX_XFER_SIZE, 'L', -1, -1, "R", "ADV" },
#endif
#ifdef _PC_REC_MIN_XFER_SIZE
	{ "_PC_REC_MIN_XFER_SIZE", _PC_REC_MIN_XFER_SIZE, 'L', -1, -1, "R", "ADV" },
#endif
#ifdef _PC_REC_XFER_ALIGN
	{ "_PC_REC_XFER_ALIGN", _PC_REC_XFER_ALIGN, 'L', -1, -1, "R", "ADV" },
#endif
#ifdef _PC_SYMLINK_MAX
	{ "_PC_SYMLINK_MAX", _PC_SYMLINK_MAX, 'L', 255, 255, "D", NULL },
#endif
#ifdef _PC_TEXTDOMAIN_MAX
	{ "_PC_TEXTDOMAIN_MAX", _PC_TEXTDOMAIN_MAX, 'L', 11, 252, "D", NULL },
#endif
#ifdef _PC_CHOWN_RESTRICTED
	{ "_PC_CHOWN_RESTRICTED", _PC_CHOWN_RESTRICTED, 'O', -1, -1, "DR", NULL },
#endif
#ifdef _PC_NO_TRUNC
	{ "_PC_NO_TRUNC", _PC_NO_TRUNC, 'O', -1, -1, "D", NULL },
#endif
#ifdef _PC_VDISABLE
	{ "_PC_VDISABLE", _PC_VDISABLE, 'O', -1, -1, "T", NULL },
#endif
#ifdef _PC_ASYNC_IO
	{ "_PC_ASYNC_IO", _PC_ASYNC_IO, 'O', -1, -1, "R", NULL },
#endif
#ifdef _PC_FALLOC
	{ "_PC_FALLOC", _PC_FALLOC, 'O', -1, -1, "R", "ADV" },
#endif
#ifdef _PC_PRIO_IO
	{ "_PC_PRIO_IO", _PC_PRIO_IO, 'O', -1, -1, "R", "PIO" },
#endif
#ifdef _PC_SYNC_IO
	{ "_PC_SYNC_IO", _PC_SYNC_IO, 'O', -1, -1, "R", "SIO" },
#endif
#ifdef _PC_TIMESTAMP_RESOLUTION
	{ "_PC_TIMESTAMP_RESOLUTION", _PC_TIMESTAMP_RESOLUTION, 'X', -1, -1, "DR", NULL },
#endif
};

/* Names of the table <unistd.h> does not define. */
static const char *const missing[] = {
#ifndef _PC_FILESIZEBITS
	"_PC_FILESIZEBITS",
#endif
#ifndef _PC_LINK_MAX
	"_PC_LINK_MAX",
#endif
#ifndef _PC_MAX_CANON
	"_PC_MAX_CANON",
#endif
#ifndef _PC_MAX_INPUT
	"_PC_MAX_INPUT",
#endif
#ifndef _PC_NAME_MAX
	"_PC_NAME_MAX",
#endif
#ifndef _PC_PATH_MAX
	"_PC_PATH_MAX",
#endif
#ifndef _PC_PIPE_BUF
	"_PC_PIPE_BUF",
#endif
#ifndef _PC_2_SYMLINKS
	"_PC_2_SYMLINKS",
#endif
#ifndef _PC_ALLOC_SIZE_MIN
	"_PC_ALLOC_SIZE_MIN",
#endif
#ifndef _PC_REC_INCR_XFER_SIZE
	"_PC_REC_INCR_XFER_SIZE",
#endif
#ifndef _PC_REC_MAX_XFER_SIZE
	"_PC_REC_MAX_XFER_SIZE",
#endif
#ifndef _PC_REC_MIN_XFER_SIZE
	"_PC_REC_MIN_XFER_SIZE",
#endif
#ifndef _PC_REC_XFER_ALIGN
	"_PC_REC_XFER_ALIGN",
#endif
#ifndef _PC_SYMLINK_MAX
	"_PC_SYMLINK_MAX",
#endif
#ifndef _PC_TEXTDOMAIN_MAX
	"_PC_TEXTDOMAIN_MAX",
#endif
#ifndef _PC_CHOWN_RESTRICTED
	"_PC_CHOWN_RESTRICTED",
#endif
#ifndef _PC_NO_TRUNC
	"_PC_NO_TRUNC",
#endif
#ifndef _PC_VDISABLE
	"_PC_VDISABLE",
#endif
#ifndef _PC_ASYNC_IO
	"_PC_ASYNC_IO",
#endif
#ifndef _PC_FALLOC
	"_PC_FALLOC",
#endif
#ifndef _PC_PRIO_IO
	"_PC_PRIO_IO",
#endif
#ifndef _PC_SYNC_IO
	"_PC_SYNC_IO",
#endif
#ifndef _PC_TIMESTAMP_RESOLUTION
	"_PC_TIMESTAMP_RESOLUTION",
#endif
	NULL
};

/* 1 if the option a variable depends on is supported. */
static int supported(const char *dep)
{
	if (dep == NULL)
		return 1;
	if (strcmp(dep, "ADV") == 0)
		return sysconf(_SC_ADVISORY_INFO) > 0;
	if (strcmp(dep, "PIO") == 0)
		return sysconf(_SC_PRIORITIZED_IO) > 0;
	if (strcmp(dep, "SIO") == 0)
		return sysconf(_SC_SYNCHRONIZED_IO) > 0;
	return 0;
}

/* Notes the names <unistd.h> lacks (a header matter, XBD <unistd.h>). */
static void note_missing(void)
{
	size_t i;

	for (i = 0; missing[i] != NULL; i++)
		pc_note("<unistd.h> does not define %s; not checked here", missing[i]);
}

#define SENTINEL 12345	/* an errno value no function sets */

int main(void)
{
	size_t i;
	long r, min;
	int e, xsi;

	pc_begin("XSH.fpathconf.004");
	note_missing();
	pc_make_dir("d");
	xsi = sysconf(_SC_XOPEN_UNIX) > 0;
	pc_test();
	for (i = 0; i < sizeof vars / sizeof vars[0]; i++) {
		if (vars[i].kind != 'L' || strchr(vars[i].where, 'D') == NULL || !supported(vars[i].dep))
			continue;
		min = xsi ? vars[i].xsi_min : vars[i].min;
		errno = SENTINEL;
		r = pathconf("d", vars[i].pc);
		e = errno;
		if (r == -1) {
			pc_check(e == SENTINEL, "pathconf() of %s on a directory failed with %s", vars[i].name,
				 pc_errname(e));
			continue;
		}
		pc_check(r >= min, "pathconf() of %s on a directory returned %ld, below the minimum acceptable value %ld%s",
			 vars[i].name, r, min, xsi ? " (XSI)" : "");
	}
	pc_pass();
}
