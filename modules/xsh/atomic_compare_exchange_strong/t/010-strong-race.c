/*
 * When several threads race to exchange 0 for their own number in a
 * fresh object, exactly one succeeds, the object holds its number, and
 * every loser's *expected receives that number.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_compare_exchange_strong.010
 */
#define PCAT_ID "XSH.atomic_compare_exchange_strong.010"
#include "pcat.h"
#ifndef PCAT_NONE

#define T 4
#define R 5000

static atomic_int cells[R];
static atomic_int wins[R];	/* successes per round */
static atomic_int winner[R];	/* the number of the thread that succeeded */
static atomic_int badlose;	/* losers whose *expected was not a rival's number */

static void *thread_main(void *arg)
{
	int me = (int)(uintptr_t)arg + 1;
	int r, exp;

	pcat_wait_go();
	for (r = 0; r < R; r++) {
		exp = 0;
		if (atomic_compare_exchange_strong(&cells[r], &exp, me)) {
			atomic_fetch_add(&wins[r], 1);
			atomic_store(&winner[r], me);
		} else if (exp < 1 || exp > T || exp == me) {
			atomic_fetch_add(&badlose, 1);
		}
	}
	return NULL;
}

int main(void)
{
	int r;

	pc_begin(PCAT_ID);
	pc_test();
	if (pcat_par(T, thread_main) != 0)
		pc_unresolved("pthread_create failed");
	for (r = 0; r < R; r++) {
		int w = atomic_load(&wins[r]);
		int c = atomic_load(&cells[r]);

		pc_check(w == 1, "round %d: %d threads succeeded", r, w);
		pc_check(c == atomic_load(&winner[r]), "round %d: object holds %d, the winner was %d", r, c,
		    atomic_load(&winner[r]));
	}
	pc_check(atomic_load(&badlose) == 0, "%d losers did not receive the winner's number", atomic_load(&badlose));
	pc_pass();
}
#endif
