/*
 * TZif files: header and layout as RFC 8536 section 3 requires.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION CONV.tzif.001
 */
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pc.h"
#include "../../../../lib/cv.h"
#include "../../../../lib/tzif.h"

static void each(const char *path, const char *rel)
{
	struct tzf f;
	size_t len;
	const char *e;
	int skip;

	(void)path;
	memset(&f, 0, sizeof f);
	f.buf = (unsigned char *)cv_slurp(path, &len);
	pc_check(f.buf != NULL, "%s cannot be read", rel);
	f.len = len;
	e = tz_layout(&f, &skip);
	if (e != NULL && skip) {
		pc_note("%s: %s (not judged)", rel, e);
		free(f.buf);
		return;
	}
	pc_check(e == NULL, "%s: %s", rel, e);
	free(f.buf);
}

int main(void)
{
	const char *dir;
	int count = 0;

	pc_begin("CONV.tzif.001");
	dir = tz_find_dir();
	if (dir == NULL)
		pc_notinuse("no time zone database directory found (TZDIR unset, none of the common locations has a UTC file)");
	pc_test();
	(void)tz_walk(dir, "", 0, 20000, &count, each);
	pc_check(count > 0, "no TZif file below %s", dir);
	pc_note("checked %d TZif files below %s", count, dir);
	pc_pass();
}
