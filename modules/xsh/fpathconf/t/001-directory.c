/*
 * pathconf() and fpathconf() answer every name whose association with
 * directories is not left unspecified.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fpathconf.001
 * MUTANT    fpathconf.dir_einval_link_max
 * MUTANT    fpathconf.nolimit_sets_errno
 * MUTANT    fpathconf.success_sets_errno
 * VARIANT   fpathconf.unassociated
 * VARIANT   fpathconf.no_limit
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
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

/*
 * Plain loops, not library calls: a call that fails must not make the test skip names.
 */
static int has(const char *set, char c)
{
	for (; *set != '\0'; set++)
		if (*set == c)
			return 1;
	return 0;
}

static int same(const char *a, const char *b)
{
	while (*a != '\0' && *a == *b) {
		a++;
		b++;
	}
	return *a == *b;
}

/* 1 if the option a variable depends on is supported. */
static int supported(const char *dep)
{
	if (dep == NULL)
		return 1;
	if (same(dep, "ADV"))
		return sysconf(_SC_ADVISORY_INFO) > 0;
	if (same(dep, "PIO"))
		return sysconf(_SC_PRIORITIZED_IO) > 0;
	if (same(dep, "SIO"))
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

/*
 * pathconf(path) and fpathconf(fd) of v: a value, or -1 for no limit (or
 * an option not provided), and errno unchanged either way.
 */
static void ask(const struct var *v, const char *path, int fd, const char *what)
{
	long r;
	int e;

	errno = SENTINEL;
	r = pathconf(path, v->pc);
	e = errno;
	pc_check(r != -1 || e == SENTINEL, "pathconf() of %s on %s failed with %s", v->name, what, pc_errname(e));
	pc_check(e == SENTINEL, "pathconf() of %s on %s returned %ld and changed errno to %s", v->name, what, r,
		 pc_errname(e));
	errno = SENTINEL;
	r = fpathconf(fd, v->pc);
	e = errno;
	pc_check(r != -1 || e == SENTINEL, "fpathconf() of %s on %s failed with %s", v->name, what, pc_errname(e));
	pc_check(e == SENTINEL, "fpathconf() of %s on %s returned %ld and changed errno to %s", v->name, what, r,
		 pc_errname(e));
}

int main(void)
{
	size_t i;
	int fd;

	pc_begin("XSH.fpathconf.001");
	note_missing();
	pc_make_dir("d");
	fd = open("d", O_RDONLY);
	pc_check(fd >= 0, "cannot open d: %s", pc_errname(errno));
	pc_test();
	for (i = 0; i < sizeof vars / sizeof vars[0]; i++)
		if (has(vars[i].where, 'D') && supported(vars[i].dep))
			ask(&vars[i], "d", fd, "a directory");
	pc_pass();
}
