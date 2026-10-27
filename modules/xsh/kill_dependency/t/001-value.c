/*
 * kill_dependency is a macro that returns the value of its argument, for
 * integer, floating, boolean and pointer values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.kill_dependency.001
 */
#define PCAT_ID "XSH.kill_dependency.001"
#include "pcat.h"
#ifndef PCAT_NONE

#ifdef kill_dependency

struct rec { int a; long b; };

static int arr[4];
static struct rec rv = { 11, 22 };

int main(void)
{
	int i = 5;
	long neg = -3;
	unsigned long long big = 0xfedcba9876543210ULL;
	double d = 2.5;
	_Bool t = 1, f = 0;
	char ch = 'q';
	int *volatile ip = &arr[2];
	struct rec *volatile rp = &rv;
	void (*volatile fp)(void) = NULL;

	pc_begin(PCAT_ID);
	pc_test();
	pc_check(kill_dependency(i) == 5, "kill_dependency(int variable) is not its value");
	pc_check(kill_dependency(7) == 7, "kill_dependency(7) is not 7");
	pc_check(kill_dependency(neg) == -3, "kill_dependency(long) is not its value");
	pc_check(kill_dependency(big) == 0xfedcba9876543210ULL, "kill_dependency(unsigned long long) is not its value");
	pc_check(kill_dependency(d) == 2.5, "kill_dependency(double) is not its value");
	pc_check(kill_dependency(t) == 1, "kill_dependency(true) is not true");
	pc_check(kill_dependency(f) == 0, "kill_dependency(false) is not false");
	pc_check(kill_dependency(ch) == 'q', "kill_dependency(char) is not its value");
	pc_check(kill_dependency(i + 1) == 6, "kill_dependency(i + 1) is not 6");
	pc_check(kill_dependency(ip) == &arr[2], "kill_dependency(int pointer) is not the pointer");
	pc_check(kill_dependency(rp)->b == 22, "kill_dependency(structure pointer) does not point to the structure");
	pc_check(kill_dependency(fp) == NULL, "kill_dependency(null function pointer) is not null");
	pc_pass();
}

#else

int main(void)
{
	pc_begin(PCAT_ID);
	pc_test();
	pc_fail("<stdatomic.h> does not define the macro kill_dependency");
}

#endif
#endif
