/*
 * Space freed by free() can be allocated again.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.free.002
 */
#include "pc.h"
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>

int main(void)
{
	struct rlimit rl;
	int i;

	pc_begin("XSH.free.002");
	/* A cap makes a leaking free() show; the cap is best effort. */
#ifdef RLIMIT_AS
	if (getrlimit(RLIMIT_AS, &rl) == 0) {
		rlim_t cap = (rlim_t)1 << 30;

		if (rl.rlim_max == RLIM_INFINITY || rl.rlim_max >= cap) {
			rl.rlim_cur = cap;
			(void)setrlimit(RLIMIT_AS, &rl);
		}
	}
#else
	(void)rl;
#endif
	pc_test();
	for (i = 0; i < 4096; i++) {
		char *p = malloc(1048576);

		pc_check(p != NULL, "malloc(1 MiB) failed in round %d: freed space was not reused", i);
		p[0] = 1;
		p[1048575] = 2;
		free(p);
	}
	pc_pass();
}
