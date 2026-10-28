/*
 * key_t is an arithmetic type, and ftok() returns one.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.11-data-types.008
 */
#include "pc.h"
#include <sys/ipc.h>
#include <sys/types.h>

int main(void)
{
	volatile double d = 1.5;
	key_t k, f;

	pc_begin("XSH.2.11-data-types.008");
	k = (key_t)d;
	f = ftok(".", 'k');
	if (f == (key_t)-1)
		pc_unresolved("ftok() failed on the working directory");
	pc_test();
	pc_check(k == 1 || (k >= 1 && k < 2), "key_t: 1.5 converted to the type is neither 1 nor in [1, 2)");
	pc_check(k + 1 > k, "key_t does not take arithmetic");
	pc_check(f + 0 == f && f - f == 0, "a key from ftok() does not take arithmetic");
	pc_check(_Generic(ftok(".", 'k'), key_t: 1, default: 0), "ftok() does not return key_t");
	pc_pass();
}
