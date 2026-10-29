/*
 * A file can have {LINK_MAX} links; one more fails with EMLINK.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.limits.h.020
 */
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "pc.h"

int main(void)
{
	long lm;
	long want, i;
	char name[32];
	struct stat st;

	pc_begin("XBD.limits.h.020");
	pc_make_file("f", "x");
	lm = pathconf("f", _PC_LINK_MAX);
	pc_check(link("f", "probe") == 0, "the file system cannot make a hard link (%s)", pc_errname(errno));
	(void)unlink("probe");
	want = lm == -1 || lm > 1024 ? 1024 : lm;
	if (want < _POSIX_LINK_MAX)
		want = _POSIX_LINK_MAX;
	pc_test();
	pc_check(lm == -1 || lm >= _POSIX_LINK_MAX, "pathconf(_PC_LINK_MAX) is %ld, below %d", lm, _POSIX_LINK_MAX);

	for (i = 2; i <= want; i++) {
		(void)snprintf(name, sizeof name, "l%ld", i);
		pc_check(link("f", name) == 0, "link number %ld failed with %s; {LINK_MAX} is %ld",
		    i, pc_errname(errno), lm);
	}
	pc_check(stat("f", &st) == 0, "stat() failed");
	pc_check((long)st.st_nlink == want, "st_nlink is %ld after %ld links", (long)st.st_nlink, want);

	if (lm != -1 && lm <= 1024) {
		(void)snprintf(name, sizeof name, "l%ld", want + 1);
		pc_expect_err(link("f", name), EMLINK, "link() past {LINK_MAX}");
		pc_check(stat("f", &st) == 0 && (long)st.st_nlink == lm,
		    "st_nlink changed after the failed link()");
	}
	for (i = 2; i <= want; i++) {
		(void)snprintf(name, sizeof name, "l%ld", i);
		(void)unlink(name);
	}
	pc_check(stat("f", &st) == 0 && st.st_nlink == 1, "st_nlink is not 1 after the links were removed");
	pc_pass();
}
