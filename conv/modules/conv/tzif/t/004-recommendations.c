/*
 * TZif files: the recommendations of RFC 8536 (SHOULD).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.tzif.004
 */
#include <sys/stat.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pc.h"
#include "../../../../lib/cv.h"
#include "../../../../lib/tzif.h"

static void shouldblock(const char *rel, const struct tzb *b, const char *which)
{
	unsigned i, k;
	const char *a;
	size_t n;

	for (i = 0; i < b->h.tim; i++)
		pc_check(tzb_trans(b, i) >= -((int64_t)1 << 59), "%s (%s): transition %u is earlier than -2^59", rel, which, i);
	for (i = 0; i < b->h.typ; i++) {
		pc_check(tzb_utoff(b, i) >= -89999 && tzb_utoff(b, i) <= 93599, "%s (%s): utoff %ld of type %u is outside [-89999, 93599]",
		    rel, which, (long)tzb_utoff(b, i), i);
		a = tzb_abbr(b, i);
		n = strlen(a);
		pc_check(n >= 3 && n <= 6, "%s (%s): designation '%s' is not 3 to 6 characters", rel, which, a);
		for (k = 0; k < n; k++)
			pc_check(isalnum((unsigned char)a[k]) || a[k] == '+' || a[k] == '-', "%s (%s): designation '%s' has a character outside alphanumerics, plus and minus", rel, which, a);
	}
}

static void each(const char *path, const char *rel)
{
	struct tzf f;
	size_t len;
	unsigned i, j, first;
	int skip;

	memset(&f, 0, sizeof f);
	f.buf = (unsigned char *)cv_slurp(path, &len);
	pc_check(f.buf != NULL, "%s cannot be read", rel);
	f.len = len;
	if (tz_layout(&f, &skip) != NULL) {
		free(f.buf);
		return;
	}
	pc_check(f.ver != 0, "%s is a version 1 file (RFC 8536 section 4: SHOULD NOT be generated)", rel);
	shouldblock(rel, &f.b2, "version 2+ block");
	pc_check(f.footlen == 0 || f.foot[0] != ':', "%s: the footer TZ string begins with a colon", rel);
	/*
	 * The version 1 transitions are a contiguous run of the version 2+ ones,
	 * with the same times. A first version 1 transition of exactly -2^31 is
	 * the range marker some writers put there, not a time change: skipped.
	 */
	first = (f.b1.h.tim > 0 && tzb_trans(&f.b1, 0) == INT32_MIN) ? 1 : 0;
	if (f.b1.h.tim > first) {
		for (j = 0; j < f.b2.h.tim && tzb_trans(&f.b2, j) != tzb_trans(&f.b1, first); j++)
			;
		pc_check(j < f.b2.h.tim && j + (f.b1.h.tim - first) <= f.b2.h.tim, "%s: the first version 1 transition is not among the version 2+ transitions", rel);
		for (i = first; i < f.b1.h.tim; i++)
			pc_check(tzb_trans(&f.b1, i) == tzb_trans(&f.b2, j + i - first), "%s: version 1 transition %u is not the next version 2+ transition", rel, i);
	}
	free(f.buf);
}

int main(void)
{
	const char *dir;
	int count = 0;

	pc_begin("CONV.tzif.004");
	dir = tz_find_dir();
	if (dir == NULL)
		pc_notinuse("no time zone database directory found (TZDIR unset, none of the common locations has a UTC file)");
	pc_test();
	(void)tz_walk(dir, "", 0, 20000, &count, each);
	pc_check(count > 0, "no TZif file below %s", dir);
	pc_note("checked %d TZif files below %s", count, dir);
	pc_pass();
}
