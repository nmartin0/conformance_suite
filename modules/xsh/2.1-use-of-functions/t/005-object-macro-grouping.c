/*
 * An object-like macro used inside an arithmetic expression acts as one operand.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.1-use-of-functions.005
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

static const char *bad;

/* M is used bare; v holds M's value (taken parenthesized). Small values. */
#define SMALL(M) do { \
	long long v = (long long)(M); \
	if (2LL * M != 2LL * v || M * 3LL != v * 3LL || 100LL - M != 100LL - v || \
	    (1000LL + M) / 2 != (1000LL + v) / 2 || 7LL * M % 5 != 7LL * v % 5 || \
	    1LL * M / 2 != 1LL * v / 2) \
		bad = #M; \
} while (0)
/* Large values: only division and remainder, which cannot overflow. */
#define LARGE(M) do { \
	long long v = (long long)(M); \
	if (M / 2 != v / 2 || M % 7 != v % 7 || M / 3 != v / 3) \
		bad = #M; \
} while (0)

int main(void)
{
	pc_begin("XSH.2.1-use-of-functions.005");
	pc_test();
	SMALL(EOF);
	SMALL(EXIT_FAILURE);
	SMALL(CHAR_BIT);
	SMALL(SIGINT);
	SMALL(SIGKILL);
	SMALL(EINVAL);
	SMALL(ENOENT);
	SMALL(SEEK_END);
	SMALL(FOPEN_MAX);
	SMALL(S_IRWXU);
	SMALL(O_CREAT);
	SMALL(BUFSIZ);
	SMALL(RAND_MAX);
	SMALL(INT_MAX);
	SMALL(INT_MIN);
	SMALL(_POSIX_VERSION);
	LARGE(LONG_MAX);
	LARGE(LONG_MIN);
	/* UINT_MAX is unsigned: convert in a way that keeps the value */
	{
		long long v = (long long)(UINT_MAX);

		if (2LL * UINT_MAX != 2LL * v || UINT_MAX / 2 != v / 2 || UINT_MAX % 7 != v % 7)
			bad = "UINT_MAX";
	}
	pc_check(bad == NULL, "%s does not act as a single operand in an expression", bad);
	pc_pass();
}
