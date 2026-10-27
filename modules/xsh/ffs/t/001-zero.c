/*
 * ffs: zero.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.ffs.001
 */
#include "pc.h"
#include <strings.h>
static int (*volatile f_ffs)(int) = ffs;
static int (*volatile f_ffsl)(long) = ffsl;
static int (*volatile f_ffsll)(long long) = ffsll;

int main(void)
{
	pc_begin("XSH.ffs.001");
	pc_test();
	pc_check(f_ffs(0) == 0, "ffs(0) was not 0");
	pc_check(f_ffsl(0L) == 0, "ffsl(0) was not 0");
	pc_check(f_ffsll(0LL) == 0, "ffsll(0) was not 0");
	pc_pass();
}
