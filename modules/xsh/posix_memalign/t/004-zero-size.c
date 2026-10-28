/*
 * posix_memalign() with size 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.posix_memalign.004
 */
#include "pc.h"
#include <stdlib.h>

int main(void)
{
	int dummy;
	void *sentinel = &dummy;
	void *a[4];
	int i, j;

	pc_begin("XSH.posix_memalign.004");
	pc_test();
	for (i = 0; i < 4; i++) {
		int rc;

		a[i] = sentinel;
		rc = posix_memalign(&a[i], 16, 0);
		if (rc != 0) {
			pc_check(rc > 0, "posix_memalign() returned %d, not an error number", rc);
			pc_check(a[i] == sentinel || a[i] == NULL, "non-zero return %d but *memptr was set to %p", rc, a[i]);
		}
		else if (a[i] == sentinel)
			pc_fail("returned 0 but *memptr was not set");
	}
	for (i = 0; i < 4; i++)
		for (j = i + 1; j < 4; j++)
			pc_check(a[i] == NULL || a[j] == NULL || a[i] != a[j] || a[i] == sentinel,
			    "two live zero-size results are the same pointer");
	for (i = 0; i < 4; i++)
		if (a[i] != sentinel)
			free(a[i]);
	pc_pass();
}
