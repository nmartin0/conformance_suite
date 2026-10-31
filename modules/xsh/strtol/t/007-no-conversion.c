/*
 * strtol: no conversion.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.strtol.007
 * MUTANT    strtol.nc_end_after_ws
 * MUTANT    strtol.nc_end_after_sign
 * MUTANT    strtol.nc_end_end
 * MUTANT    strtol.nc_end_unset
 * MUTANT    strtol.nc_nonzero
 * MUTANT    strtol.nc_errno_erange
 * MUTANT    strtol.nc_errno_edom
 * MUTANT    strtol.space_after_sign
 * VARIANT   strtol.nc_einval
 */
#define IC_L
#include "ic.h"
#include <string.h>

static const char *const bad[] = { "", "   ", "xyz", "+", "-", "+ 1" };

int main(void)
{
	int j, k, err;
	char *e;
	R r;
	pc_begin("XSH.strtol.007");
	pc_test();
	for (j = 0; j < IC_N; j++)
		for (k = 0; k < (int)(sizeof bad / sizeof bad[0]); k++) {
			e = NULL;
			errno = 0;
			r = ic_fns[j].f(bad[k], &e, 10);
			err = errno;
			pc_check(r == 0, "%s(\"%s\") returned non-zero", ic_fns[j].name, bad[k]);
			pc_check(e == bad[k], "%s(\"%s\") did not store the start of the string in the end pointer", ic_fns[j].name, bad[k]);
			pc_check(err == 0 || err == EINVAL, "%s(\"%s\") set errno to %s", ic_fns[j].name, bad[k], pc_errname(err));
		}
	pc_pass();
}
