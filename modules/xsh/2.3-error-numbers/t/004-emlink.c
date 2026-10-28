/*
 * A link that would push the link count above {LINK_MAX} fails with EMLINK.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.3-error-numbers.004
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

static void on_alarm(int s)
{
	(void)s;
	pc_fail("the test timed out");
}

int main(void)
{
	long lim, i;
	struct stat st;
	char name[32];
	int saved = 0;

	pc_begin("XSH.2.3-error-numbers.004");
	pc_make_file("base", "x");
	errno = 0;
	lim = pathconf("base", _PC_LINK_MAX);
	if (lim == -1)
		pc_notinuse("{LINK_MAX} is indeterminate for the file");
	if (lim > 70000)
		pc_notinuse("{LINK_MAX} is %ld: too many links to make", lim);
	if (lim < 2)
		pc_unresolved("{LINK_MAX} is %ld", lim);
	pc_require_space();
	(void)signal(SIGALRM, on_alarm);
	pc_test();
	(void)alarm(100);
	for (i = 1; i <= lim + 10; i++) {
		(void)snprintf(name, sizeof name, "l%ld", i);
		if (link("base", name) == -1) {
			saved = errno;
			break;
		}
	}
	if (saved == 0)
		pc_fail("link() still succeeded with %ld links (limit %ld)", i - 1, lim);
	if (stat("base", &st) != 0)
		pc_unresolved("stat failed");
	if (saved != EMLINK && (long)st.st_nlink < lim)
		pc_unresolved("link() failed with %s before the limit (count %ld, limit %ld)",
		    pc_errname(saved), (long)st.st_nlink, lim);
	pc_check(saved == EMLINK, "link() at the limit failed with %s, not EMLINK", pc_errname(saved));
	pc_check((long)st.st_nlink == lim, "the link count is %ld at the failure, expected %ld",
	    (long)st.st_nlink, lim);
	pc_pass();
}
