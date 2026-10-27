/*
 * atomic_compare_exchange_strong() on atomic pointer objects: both
 * outcomes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_compare_exchange_strong.003
 */
#define PCAT_ID "XSH.atomic_compare_exchange_strong.003"
#include "pcat.h"
#ifndef PCAT_NONE

struct rec { int a; long b; char c[3]; };

static int arr[4];
static struct rec recs[2];

int main(void)
{
	_Atomic(int *) pi = &arr[1];
	_Atomic(struct rec *) pr = NULL;
	_Atomic(int *) *volatile ppi = &pi;
	_Atomic(struct rec *) *volatile ppr = &pr;
	int *ei, *gi;
	struct rec *er, *gr;
	_Bool r;

	pc_begin(PCAT_ID);
	pc_test();
	ei = &arr[1];
	r = atomic_compare_exchange_strong(ppi, &ei, &arr[3]);
	gi = *ppi;
	pc_check(r, "int pointer: equal pointers gave false");
	pc_check(gi == &arr[3], "int pointer: desired pointer not stored");
	pc_check(ei == &arr[1], "int pointer: *expected changed on success");
	ei = &arr[0];
	r = atomic_compare_exchange_strong(ppi, &ei, &arr[2]);
	gi = *ppi;
	pc_check(!r, "int pointer: different pointers gave true");
	pc_check(gi == &arr[3], "int pointer: object changed on failure");
	pc_check(ei == &arr[3], "int pointer: *expected not updated on failure");
	er = NULL;
	r = atomic_compare_exchange_strong_explicit(ppr, &er, &recs[1], memory_order_acq_rel, memory_order_acquire);
	gr = *ppr;
	pc_check(r && gr == &recs[1] && er == NULL, "structure pointer: null expected not exchanged");
	er = NULL;
	r = atomic_compare_exchange_strong_explicit(ppr, &er, &recs[0], memory_order_release, memory_order_relaxed);
	gr = *ppr;
	pc_check(!r && gr == &recs[1] && er == &recs[1], "structure pointer: failure path wrong");
	pc_pass();
}
#endif
