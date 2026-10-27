/*
 * atomic_init() on atomic pointer objects gives them the pointer, and the
 * objects then work.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_init.003
 */
#define PCAT_ID "XSH.atomic_init.003"
#include "pcat.h"
#ifndef PCAT_NONE

struct rec { int a; long b; char c[3]; };

static int arr[4];
static struct rec recs[2];
static void fn(void) { }

int main(void)
{
	_Atomic(int *) pi;
	_Atomic(struct rec *) pr;
	_Atomic(void (*)(void)) pf;
	_Atomic(int *) *volatile ppi = &pi;
	_Atomic(struct rec *) *volatile ppr = &pr;
	_Atomic(void (*)(void)) *volatile ppf = &pf;
	int *gi;
	struct rec *gr;
	void (*gf)(void);

	pc_begin(PCAT_ID);
	memset((void *)&pi, 0xa5, sizeof pi);
	memset((void *)&pr, 0xa5, sizeof pr);
	memset((void *)&pf, 0xa5, sizeof pf);
	pc_test();
	atomic_init(ppi, &arr[2]);
	gi = *ppi;
	pc_check(gi == &arr[2], "int pointer not held after atomic_init");
	pc_check(atomic_exchange(ppi, &arr[1]) == &arr[2], "int pointer: exchange did not return the initial pointer");
	atomic_init(ppr, NULL);
	gr = *ppr;
	pc_check(gr == NULL, "null pointer not held after atomic_init");
	pc_check(atomic_exchange(ppr, &recs[1]) == NULL, "structure pointer: exchange did not return the null pointer");
	atomic_init(ppf, fn);
	gf = *ppf;
	pc_check(gf == fn, "function pointer not held after atomic_init");
	pc_check(atomic_load(ppf) == fn, "function pointer not loaded after atomic_init");
	pc_pass();
}
#endif
