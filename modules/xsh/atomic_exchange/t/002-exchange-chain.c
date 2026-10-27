/*
 * atomic_exchange() on pointer objects, and in a chain: each call returns
 * what the one before installed, including when the value is unchanged.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_exchange.002
 */
#define PCAT_ID "XSH.atomic_exchange.002"
#include "pcat.h"
#ifndef PCAT_NONE

struct rec { int a; long b; char c[3]; };

static int arr[4];
static struct rec recs[2];

int main(void)
{
	_Atomic(int *) pi = &arr[0];
	_Atomic(struct rec *) pr = NULL;
	_Atomic(int *) *volatile ppi = &pi;
	_Atomic(struct rec *) *volatile ppr = &pr;
	atomic_long num = 5;
	atomic_long *volatile pn = &num;
	int *gi;
	struct rec *gr;

	pc_begin(PCAT_ID);
	pc_test();
	gi = atomic_exchange(ppi, &arr[2]);
	pc_check(gi == &arr[0], "int pointer: previous pointer not returned");
	gi = atomic_exchange(ppi, &arr[3]);
	pc_check(gi == &arr[2], "int pointer: second call did not return the first call's pointer");
	gi = atomic_exchange(ppi, &arr[3]);
	pc_check(gi == &arr[3], "int pointer: exchange with the same pointer did not return it");
	gi = *ppi;
	pc_check(gi == &arr[3], "int pointer: object holds the wrong pointer");
	gr = atomic_exchange_explicit(ppr, &recs[1], memory_order_acq_rel);
	pc_check(gr == NULL, "structure pointer: previous null pointer not returned");
	gr = atomic_exchange(ppr, NULL);
	pc_check(gr == &recs[1], "structure pointer: previous pointer not returned");
	gr = *ppr;
	pc_check(gr == NULL, "structure pointer: object does not hold the null pointer");
	pc_check(atomic_exchange(pn, 6) == 5, "chain: first call did not return 5");
	pc_check(atomic_exchange(pn, 7) == 6, "chain: second call did not return 6");
	pc_check(atomic_exchange(pn, 7) == 7, "chain: same-value call did not return 7");
	pc_check(atomic_exchange(pn, -1) == 7, "chain: next call did not return 7");
	pc_check(atomic_exchange(pn, 0) == -1, "chain: last call did not return -1");
	pc_pass();
}
#endif
