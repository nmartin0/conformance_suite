/*
 * ATOMIC_VAR_INIT(value) initializes atomic objects (automatic and
 * static, integer and pointer) to the value.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_init.004
 */
#define PCAT_ID "XSH.atomic_init.004"
#include "pcat.h"
#ifndef PCAT_NONE

#ifdef ATOMIC_VAR_INIT

static int target;
static atomic_long slong = ATOMIC_VAR_INIT(-3);
static atomic_uint suint = ATOMIC_VAR_INIT(4000000000u);
static _Atomic(int *) sptr = ATOMIC_VAR_INIT(&target);

int main(void)
{
	atomic_int a = ATOMIC_VAR_INIT(7);
	atomic_bool b = ATOMIC_VAR_INIT(1);
	atomic_ullong w = ATOMIC_VAR_INIT(0x123456789ULL);
	atomic_char c = ATOMIC_VAR_INIT('x');
	_Atomic(int *) ptr = ATOMIC_VAR_INIT(&target);
	atomic_int *volatile pa = &a;
	atomic_bool *volatile pb = &b;
	atomic_ullong *volatile pw = &w;
	atomic_char *volatile pc = &c;
	_Atomic(int *) *volatile pp = &ptr;
	_Atomic(int *) *volatile psp = &sptr;
	atomic_long *volatile pl = &slong;
	atomic_uint *volatile pu = &suint;

	pc_begin(PCAT_ID);
	pc_test();
	pc_check(atomic_load(pa) == 7, "automatic atomic_int is not 7");
	pc_check(atomic_load(pb) == 1, "automatic atomic_bool is not true");
	pc_check(atomic_load(pw) == 0x123456789ULL, "automatic atomic_ullong is wrong");
	pc_check(atomic_load(pc) == 'x', "automatic atomic_char is wrong");
	pc_check(atomic_load(pp) == &target, "automatic atomic pointer is wrong");
	pc_check(atomic_load(psp) == &target, "static atomic pointer is wrong");
	pc_check(atomic_load(pl) == -3, "static atomic_long is not -3");
	pc_check(atomic_load(pu) == 4000000000u, "static atomic_uint is wrong");
	pc_pass();
}

#else

int main(void)
{
	pc_begin(PCAT_ID);
	pc_test();
	pc_fail("<stdatomic.h> does not define ATOMIC_VAR_INIT");
}

#endif
#endif
