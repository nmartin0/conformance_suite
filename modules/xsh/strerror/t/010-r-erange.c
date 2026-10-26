/*
 * strerror_r() gives ERANGE when the buffer is too small, and keeps to it.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strerror.010
 * MUTANT    strerror.r_no_erange
 * MUTANT    strerror.r_overflow
 * MUTANT    strerror.r_off_by_one
 * MUTANT    strerror.r_minus1
 * VARIANT   strerror.r_untouched
 * VARIANT   strerror.r_errno
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define GUARD 64

static char big[65536];

/* 1 if p[0..n) are all 'Z'. */
static int intact(const char *p, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++)
		if (p[i] != 'Z')
			return 0;
	return 1;
}

struct one {
	int e;
	const char *name;
	char msg[4096];
	size_t len;
	char *b;
};

/* Setup (planted faults are off): the message and its length L. */
static void prepare(struct one *o)
{
	pc_check(strerror_r(o->e, big, sizeof big) == 0, "strerror_r(%s) failed with a large buffer", o->name);
	o->len = strlen(big);
	pc_check(o->len > 0, "strerror_r(%s) stored an empty string", o->name);
	pc_check(o->len < sizeof o->msg, "the %s message is longer than %zu bytes", o->name, sizeof o->msg - 1);
	memcpy(o->msg, big, o->len + 1);
	o->b = malloc(o->len + 1 + GUARD);
	pc_check(o->b != NULL, "malloc failed");
}

static void check(struct one *o)
{
	size_t len = o->len, sizes[3];
	char *b = o->b;
	int r, k;

	memset(b, 'Z', len + 1 + GUARD);
	r = strerror_r(o->e, b, len + 1);
	pc_check(r == 0, "strerror_r(%s, buf, %zu) returned %d (%s), expected 0", o->name, len + 1, r, pc_errname(r));
	pc_check(memcmp(b, o->msg, len + 1) == 0, "strerror_r(%s, buf, %zu) stored another string", o->name, len + 1);
	pc_check(intact(b + len + 1, GUARD), "strerror_r(%s, buf, %zu) wrote past the buffer", o->name, len + 1);

	sizes[0] = len;
	sizes[1] = 1;
	sizes[2] = 0;
	for (k = 0; k < 3; k++) {
		memset(b, 'Z', len + 1 + GUARD);
		r = strerror_r(o->e, b, sizes[k]);
		pc_check(r == ERANGE, "strerror_r(%s, buf, %zu) for a %zu-byte message returned %d (%s), expected ERANGE",
		    o->name, sizes[k], len, r, pc_errname(r));
		pc_check(intact(b + sizes[k], len + 1 + GUARD - sizes[k]),
		    "strerror_r(%s, buf, %zu) wrote at or past buf[%zu]", o->name, sizes[k], sizes[k]);
	}
}

int main(void)
{
	static struct one a = { ENOENT, "ENOENT", "", 0, NULL }, b = { EINVAL, "EINVAL", "", 0, NULL };

	pc_begin("XSH.strerror.010");
	prepare(&a);
	prepare(&b);
	pc_test();
	check(&a);
	check(&b);
	pc_pass();
}
