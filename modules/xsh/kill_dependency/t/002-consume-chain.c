/*
 * A pointer published with a release store and read with a consume load
 * leads to the published data; kill_dependency() of the value read
 * through it is that data.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.kill_dependency.002
 */
#define PCAT_ID "XSH.kill_dependency.002"
#include "pcat.h"
#ifndef PCAT_NONE

#ifdef kill_dependency

#define N 2000

struct node { int val; };

static struct node nodes[N];		/* ordinary memory */
static _Atomic(struct node *) slots[N];
static atomic_int bad, timedout;

static void *thread_main(void *arg)
{
	int idx = (int)(uintptr_t)arg;
	int k;

	pcat_wait_go();
	if (idx == 0) {
		for (k = 0; k < N; k++) {
			nodes[k].val = k + 100;
			atomic_store_explicit(&slots[k], &nodes[k], memory_order_release);
		}
	} else {
		long long t0 = pcat_ms();
		unsigned n = 0;

		for (k = 0; k < N; k++) {
			struct node *p;

			while ((p = atomic_load_explicit(&slots[k], memory_order_consume)) == NULL) {
				pcat_relax(&n);
				if (pcat_expired(t0, 20000)) {
					atomic_store(&timedout, 1);
					return NULL;
				}
			}
			if (p != &nodes[k] || kill_dependency(p->val) != k + 100)
				atomic_fetch_add(&bad, 1);
		}
	}
	return NULL;
}

int main(void)
{
	int k;

	pc_begin(PCAT_ID);
	for (k = 0; k < N; k++)
		atomic_store(&slots[k], NULL);
	pc_test();
	if (pcat_par(2, thread_main) != 0)
		pc_unresolved("pthread_create failed");
	pc_check(atomic_load(&bad) == 0, "%d reads through a consume-loaded pointer gave the wrong data", atomic_load(&bad));
	if (atomic_load(&timedout))
		pc_unresolved("a published pointer was not seen within 20 s");
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
