/*
 * pathconf() values agree with <limits.h> and <unistd.h>.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fpathconf.005
 * MUTANT    fpathconf.name_max_14
 * MUTANT    fpathconf.no_trunc_off
 * VARIANT   fpathconf.no_limit
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SENTINEL 12345

/* Compile-time values of <limits.h> for the pathname variables. */
static const struct {
	const char *macro;
	int pc;
	long value;
} limits[] = {
#if defined(FILESIZEBITS) && defined(_PC_FILESIZEBITS)
	{ "FILESIZEBITS", _PC_FILESIZEBITS, (long)(FILESIZEBITS) },
#endif
#if defined(LINK_MAX) && defined(_PC_LINK_MAX)
	{ "LINK_MAX", _PC_LINK_MAX, (long)(LINK_MAX) },
#endif
#if defined(NAME_MAX) && defined(_PC_NAME_MAX)
	{ "NAME_MAX", _PC_NAME_MAX, (long)(NAME_MAX) },
#endif
#if defined(PATH_MAX) && defined(_PC_PATH_MAX)
	{ "PATH_MAX", _PC_PATH_MAX, (long)(PATH_MAX) },
#endif
#if defined(PIPE_BUF) && defined(_PC_PIPE_BUF)
	{ "PIPE_BUF", _PC_PIPE_BUF, (long)(PIPE_BUF) },
#endif
#if defined(SYMLINK_MAX) && defined(_PC_SYMLINK_MAX)
	{ "SYMLINK_MAX", _PC_SYMLINK_MAX, (long)(SYMLINK_MAX) },
#endif
#if defined(TEXTDOMAIN_MAX) && defined(_PC_TEXTDOMAIN_MAX)
	{ "TEXTDOMAIN_MAX", _PC_TEXTDOMAIN_MAX, (long)(TEXTDOMAIN_MAX) },
#endif
	{ NULL, 0, 0 }
};

/* pathconf(path, pc), with -1 and errno unchanged reported as "no limit". */
static long ask(const char *path, int pc, int *nolimit)
{
	long r;

	errno = SENTINEL;
	r = pathconf(path, pc);
	*nolimit = r == -1 && errno == SENTINEL;
	return r;
}

int main(void)
{
	size_t i;
	long r;
	int nl;

	pc_begin("XSH.fpathconf.005");
	pc_make_dir("d");
	pc_make_file("f", "data");
	pc_test();
	for (i = 0; limits[i].macro != NULL; i++) {
		r = ask("d", limits[i].pc, &nl);
		if (nl)
			continue;
		pc_check(r >= limits[i].value, "pathconf() on a directory returned %ld, below <limits.h>'s %s %ld", r,
			 limits[i].macro, limits[i].value);
	}
#if defined(_POSIX_NO_TRUNC) && defined(_PC_NO_TRUNC)
	if (_POSIX_NO_TRUNC > 0) {
		r = ask("d", _PC_NO_TRUNC, &nl);
		pc_check(r != -1, "_POSIX_NO_TRUNC is %ld but pathconf(_PC_NO_TRUNC) on a directory returns -1",
			 (long)_POSIX_NO_TRUNC);
	}
#endif
#if defined(_POSIX_CHOWN_RESTRICTED) && defined(_PC_CHOWN_RESTRICTED)
	if (_POSIX_CHOWN_RESTRICTED > 0) {
		r = ask("d", _PC_CHOWN_RESTRICTED, &nl);
		pc_check(r != -1, "_POSIX_CHOWN_RESTRICTED is %ld but pathconf(_PC_CHOWN_RESTRICTED) on a directory "
			 "returns -1", (long)_POSIX_CHOWN_RESTRICTED);
	}
#endif
#if defined(_POSIX_TIMESTAMP_RESOLUTION) && defined(_PC_TIMESTAMP_RESOLUTION)
	r = ask("d", _PC_TIMESTAMP_RESOLUTION, &nl);
	pc_check(r == _POSIX_TIMESTAMP_RESOLUTION, "pathconf(_PC_TIMESTAMP_RESOLUTION) on a directory returned %ld, "
		 "<unistd.h> defines %ld for all files", r, (long)_POSIX_TIMESTAMP_RESOLUTION);
	r = ask("f", _PC_TIMESTAMP_RESOLUTION, &nl);
	pc_check(r == _POSIX_TIMESTAMP_RESOLUTION, "pathconf(_PC_TIMESTAMP_RESOLUTION) on a regular file returned "
		 "%ld, <unistd.h> defines %ld for all files", r, (long)_POSIX_TIMESTAMP_RESOLUTION);
#endif
#if defined(_POSIX2_SYMLINKS) && defined(_PC_2_SYMLINKS)
	r = ask("d", _PC_2_SYMLINKS, &nl);
	pc_check(r == _POSIX2_SYMLINKS, "pathconf(_PC_2_SYMLINKS) on a directory returned %ld, <unistd.h> defines "
		 "%ld for all paths", r, (long)_POSIX2_SYMLINKS);
#endif
#if defined(_POSIX_ASYNC_IO) && defined(_PC_ASYNC_IO)
	r = ask("f", _PC_ASYNC_IO, &nl);
	if (_POSIX_ASYNC_IO == -1)
		pc_check(r == -1, "_POSIX_ASYNC_IO is -1 (the option on no file) but pathconf(_PC_ASYNC_IO) on a regular file returned %ld", r);
	else
		pc_check(r != -1, "_POSIX_ASYNC_IO is %ld (the option on all files) but pathconf(_PC_ASYNC_IO) on a regular file returned -1",
			 (long)_POSIX_ASYNC_IO);
#endif
#if defined(_POSIX_FALLOC) && defined(_PC_FALLOC)
	r = ask("f", _PC_FALLOC, &nl);
	if (_POSIX_FALLOC == -1)
		pc_check(r == -1, "_POSIX_FALLOC is -1 (the option on no file) but pathconf(_PC_FALLOC) on a regular file returned %ld", r);
	else
		pc_check(r != -1, "_POSIX_FALLOC is %ld (the option on all files) but pathconf(_PC_FALLOC) on a regular file returned -1",
			 (long)_POSIX_FALLOC);
#endif
#if defined(_POSIX_PRIO_IO) && defined(_PC_PRIO_IO)
	r = ask("f", _PC_PRIO_IO, &nl);
	if (_POSIX_PRIO_IO == -1)
		pc_check(r == -1, "_POSIX_PRIO_IO is -1 (the option on no file) but pathconf(_PC_PRIO_IO) on a regular file returned %ld", r);
	else
		pc_check(r != -1, "_POSIX_PRIO_IO is %ld (the option on all files) but pathconf(_PC_PRIO_IO) on a regular file returned -1",
			 (long)_POSIX_PRIO_IO);
#endif
#if defined(_POSIX_SYNC_IO) && defined(_PC_SYNC_IO)
	r = ask("f", _PC_SYNC_IO, &nl);
	if (_POSIX_SYNC_IO == -1)
		pc_check(r == -1, "_POSIX_SYNC_IO is -1 (the option on no file) but pathconf(_PC_SYNC_IO) on a regular file returned %ld", r);
	else
		pc_check(r != -1, "_POSIX_SYNC_IO is %ld (the option on all files) but pathconf(_PC_SYNC_IO) on a regular file returned -1",
			 (long)_POSIX_SYNC_IO);
#endif
	pc_pass();
}
