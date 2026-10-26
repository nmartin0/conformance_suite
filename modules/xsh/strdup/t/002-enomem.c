/*
 * strdup() gives ENOMEM when the copy does not fit.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strdup.002
 * MUTANT    strdup.enomem_errno
 * MUTANT    strdup.enomem_static
 */
#include "pc.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>

#define LIMIT ((rlim_t)384 << 20)
#define BIG ((size_t)224 << 20)

/* Setup: lower RLIMIT_AS to LIMIT; UNRESOLVED when that is not possible. */
static void lower_as(void)
{
	struct rlimit rl;

	pc_check(getrlimit(RLIMIT_AS, &rl) == 0, "getrlimit(RLIMIT_AS) failed: %s", pc_errname(errno));
	if (rl.rlim_max != RLIM_INFINITY && rl.rlim_max < LIMIT)
		pc_unresolved("the RLIMIT_AS hard limit is below 384 MiB");
	rl.rlim_cur = LIMIT;
	pc_check(setrlimit(RLIMIT_AS, &rl) == 0, "setrlimit(RLIMIT_AS) failed: %s", pc_errname(errno));
}

int main(void)
{
	char *s, *p;
	int e;

	pc_begin("XSH.strdup.002");
	lower_as();
	s = malloc(BIG + 1);
	if (s == NULL)
		pc_unresolved("a %zu-byte string cannot be allocated under the limit", BIG + 1);
	memset(s, 'a', BIG);
	s[BIG] = '\0';
	pc_test();
	errno = 0;
	p = strdup(s);
	e = errno;
	pc_check(p == NULL, "strdup() of a %zu-byte string under a %d MiB address space limit succeeded",
	    BIG, (int)(LIMIT >> 20));
	pc_check(e == ENOMEM, "strdup() failed with errno %s, expected ENOMEM", pc_errname(e));
	pc_pass();
}
