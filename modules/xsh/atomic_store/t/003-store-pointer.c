/*
 * atomic_store() and atomic_store_explicit() replace the pointer held by
 * an atomic pointer object.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_store.003
 */
#define PCAT_ID "XSH.atomic_store.003"
#include "pcat.h"
#ifndef PCAT_NONE

struct rec { int a; long b; char c[3]; };

static int arr[4];
static struct rec recs[2];
static void fn(void) { }

int main(void)
{
	_Atomic(int *) pi = NULL;
	_Atomic(struct rec *) pr = &recs[0];
	_Atomic(void (*)(void)) pf = NULL;
	_Atomic(int *) *volatile ppi = &pi;
	_Atomic(struct rec *) *volatile ppr = &pr;
	_Atomic(void (*)(void)) *volatile ppf = &pf;
	int *gi;
	struct rec *gr;
	void (*gf)(void);

	pc_begin(PCAT_ID);
	pc_test();
	atomic_store(ppi, &arr[3]);
	gi = *ppi;
	pc_check(gi == &arr[3], "int pointer not stored by atomic_store");
	atomic_store_explicit(ppi, NULL, memory_order_release);
	gi = *ppi;
	pc_check(gi == NULL, "null pointer not stored by atomic_store_explicit");
	atomic_store_explicit(ppr, &recs[1], memory_order_relaxed);
	gr = *ppr;
	pc_check(gr == &recs[1], "structure pointer not stored by atomic_store_explicit");
	atomic_store(ppf, fn);
	gf = *ppf;
	pc_check(gf == fn, "function pointer not stored");
	pc_pass();
}
#endif
