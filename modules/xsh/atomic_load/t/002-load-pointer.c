/*
 * atomic_load() on atomic pointer objects returns the pointer held.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_load.002
 */
#define PCAT_ID "XSH.atomic_load.002"
#include "pcat.h"
#ifndef PCAT_NONE

struct rec { int a; long b; char c[3]; };

static int arr[4];
static char buf[8];
static struct rec recs[2];
static void fn(void) { }

int main(void)
{
	_Atomic(int *) pi = &arr[2];
	_Atomic(char *) pc = &buf[5];
	_Atomic(void *) pv = (void *)&recs[1];
	_Atomic(struct rec *) pr = &recs[1];
	_Atomic(int *) pn = NULL;
	_Atomic(void (*)(void)) pf = fn;
	_Atomic(int *) *volatile ppi = &pi;
	_Atomic(char *) *volatile ppc = &pc;
	_Atomic(void *) *volatile ppv = &pv;
	_Atomic(struct rec *) *volatile ppr = &pr;
	_Atomic(int *) *volatile ppn = &pn;
	_Atomic(void (*)(void)) *volatile ppf = &pf;

	pc_begin(PCAT_ID);
	pc_test();
	pc_check(atomic_load(ppi) == &arr[2], "int pointer not returned");
	pc_check(atomic_load(ppc) == &buf[5], "char pointer not returned");
	pc_check(atomic_load(ppv) == (void *)&recs[1], "void pointer not returned");
	pc_check(atomic_load(ppr) == &recs[1], "structure pointer not returned");
	pc_check(atomic_load(ppn) == NULL, "null pointer not returned");
	pc_check(atomic_load(ppf) == fn, "function pointer not returned");
	pc_pass();
}
#endif
