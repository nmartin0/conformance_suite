/*
 * strtol: bad base.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtol.012
 * MUTANT    strtol.base37_accepted
 * MUTANT    strtol.base1_accepted
 * MUTANT    strtol.negbase_accepted
 * MUTANT    strtol.bad_base_erange
 * MUTANT    strtol.bad_base_no_errno
 * READING   strtol.base37_means_37
 */
#define IC_L
#include "ic.h"
#include <string.h>

static const int bad[3] = { 37, 1, -1 };

int main(void)
{
	int j, k, err;
	pc_begin("XSH.strtol.012");
	pc_test();
	for (j = 0; j < IC_N; j++)
		for (k = 0; k < 3; k++) {
			errno = 0;
			(void)ic_fns[j].f("10", NULL, bad[k]);
			err = errno;
			pc_check(err == EINVAL, "%s() with base %d set errno to %s, expected EINVAL", ic_fns[j].name, bad[k], pc_errname(err));
		}
	pc_pass();
}
