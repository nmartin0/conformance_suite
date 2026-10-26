/*
 * pathconf() and fpathconf() of a value that is no name fail with
 * EINVAL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fpathconf.008
 * MUTANT    fpathconf.invalid_returns_zero
 * MUTANT    fpathconf.invalid_wrong_errno
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

int main(void)
{
	static const int bad[] = { -1, 0x7ffffff0 };
	size_t i, j;
	long r;
	int fd, e;

	pc_begin("XSH.fpathconf.008");
	for (i = 0; i < 2; i++)
		for (j = 0; j < sizeof vars / sizeof vars[0]; j++)
			pc_check(vars[j].pc != bad[i], "%s has the value %d", vars[j].name, bad[i]);
	pc_make_dir("d");
	fd = open("d", O_RDONLY);
	pc_check(fd >= 0, "cannot open d: %s", pc_errname(errno));
	pc_test();
	for (i = 0; i < 2; i++) {
		errno = 0;
		r = pathconf("d", bad[i]);
		e = errno;
		pc_check(r == -1 && e == EINVAL, "pathconf(\"d\", %d) returned %ld (errno %s), expected -1 and EINVAL",
			 bad[i], r, pc_errname(e));
		errno = 0;
		r = fpathconf(fd, bad[i]);
		e = errno;
		pc_check(r == -1 && e == EINVAL, "fpathconf(fd, %d) returned %ld (errno %s), expected -1 and EINVAL",
			 bad[i], r, pc_errname(e));
	}
	pc_pass();
}
