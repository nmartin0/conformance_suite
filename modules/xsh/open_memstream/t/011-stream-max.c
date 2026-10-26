/*
 * open_memstream() fails with EMFILE when {STREAM_MAX} streams are open.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open_memstream.011
 * MUTANT    open_memstream.no_limit
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <unistd.h>
#include <wchar.h>

/* After pc_test(): a step that is not under test gives UNRESOLVED, not FAIL. */
#define NEED(cond, ...) do { if (!(cond)) pc_unresolved(__VA_ARGS__); } while (0)

/* A dynamic memory stream, which must open (the call under test). */
static FILE *ms(char **b, size_t *n)
{
	FILE *f;

	errno = 0;
	f = open_memstream(b, n);
	pc_check(f != NULL, "open_memstream() failed: %s", pc_errname(errno));
	return f;
}

/* Bytes as text for messages (printable or \ooo). */
static const char *show(const void *p, size_t n)
{
	static char s[256];
	const unsigned char *b = p;
	size_t i, k = 0;

	for (i = 0; i < n && k + 5 < sizeof s; i++)
		k += (size_t)snprintf(s + k, sizeof s - k, b[i] >= 0x20 && b[i] < 0x7f ? "%c" : "\\%03o", b[i]);
	s[k] = '\0';
	return s;
}

/*
 * Opens streams until {STREAM_MAX} are open (stdin, stdout and stderr
 * count). An indeterminate limit is NOTINUSE; one that follows
 * RLIMIT_NOFILE, or is too large to reach, is NOTINUSE too.
 */
static long fill_streams(void)
{
	struct rlimit rl;
	long max, i;

	errno = 0;
	max = sysconf(_SC_STREAM_MAX);
	if (max == -1 && errno == 0)
		pc_notinuse("{STREAM_MAX} is indeterminate");
	if (max < 8)
		pc_unresolved("sysconf(_SC_STREAM_MAX) gave %ld", max);
	if (max > 4096) {
		/* It may follow RLIMIT_NOFILE (where the limit follows RLIMIT_NOFILE): see whether a lower limit lowers it. */
		pc_check(getrlimit(RLIMIT_NOFILE, &rl) == 0, "getrlimit failed");
		rl.rlim_cur = 256;
		if (setrlimit(RLIMIT_NOFILE, &rl) == 0 && sysconf(_SC_STREAM_MAX) != max)
			pc_notinuse("{STREAM_MAX} follows RLIMIT_NOFILE");
		pc_notinuse("{STREAM_MAX} is %ld, too many to open", max);
	}
	/* Descriptors must not run out first. */
	pc_check(getrlimit(RLIMIT_NOFILE, &rl) == 0, "getrlimit failed");
	if (rl.rlim_cur != RLIM_INFINITY && rl.rlim_cur < (rlim_t)max + 16) {
		if (rl.rlim_max != RLIM_INFINITY && rl.rlim_max < (rlim_t)max + 16)
			pc_unresolved("RLIMIT_NOFILE (%ld) is below {STREAM_MAX} (%ld)", (long)rl.rlim_max, max);
		rl.rlim_cur = (rlim_t)max + 16;
		if (!(setrlimit(RLIMIT_NOFILE, &rl) == 0))
			pc_unresolved("setrlimit failed");
		if (sysconf(_SC_STREAM_MAX) != max)
			pc_notinuse("{STREAM_MAX} follows RLIMIT_NOFILE");
	}
	for (i = 3; i < max; i++) {
		errno = 0;
		if (fopen("s", "r") == NULL)
			pc_unresolved("only %ld streams could be opened, below {STREAM_MAX} (%ld): %s", i, max, pc_errname(errno));
	}
	return max;
}

int main(void)
{
	pc_begin("XSH.open_memstream.011");
	pc_make_file("s", "x");
	(void)fill_streams();	/* before the test phase: no planted fault applies to it */
	pc_test();
	{
		char *b = NULL;
		size_t n = 0;
		FILE *f;
		int e;

		errno = 0;
		f = open_memstream(&b, &n);
		e = errno;
		pc_check(f == NULL, "open_memstream() succeeded with {STREAM_MAX} streams open");
		pc_check(e == EMFILE, "open_memstream() failed with %s, expected EMFILE", pc_errname(e));
	}
	pc_pass();
}
