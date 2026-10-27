/*
 * gethostname() gives a null-terminated name within the host name limit, the same on a second call.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.gethostname.001
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	char a[4096], b[4096];
	long lim;

	pc_begin("XSH.gethostname.001");
	memset(a, 0xA5, sizeof a);
	memset(b, 0xA5, sizeof b);
	lim = sysconf(_SC_HOST_NAME_MAX);
#ifdef HOST_NAME_MAX
	if (lim == -1)
		lim = HOST_NAME_MAX;
#endif
	pc_test();
	pc_check(gethostname(a, sizeof a) == 0, "gethostname() failed: %s", pc_errname(errno));
	pc_check(memchr(a, '\0', sizeof a) != NULL, "the name is not null-terminated");
	if (lim >= 0)
		pc_check((long)strlen(a) <= lim, "the name has %lu characters, over the limit %ld", (unsigned long)strlen(a), lim);
	pc_check(gethostname(b, sizeof b) == 0, "the second gethostname() failed: %s", pc_errname(errno));
	pc_check(strcmp(a, b) == 0, "two calls gave \"%s\" and \"%s\"", a, b);
	pc_pass();
}
