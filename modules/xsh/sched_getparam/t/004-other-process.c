/*
 * sched_getparam() of another process returns that process's priority.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_getparam.004
 */
#include "schedh.h"

int main(void)
{
	struct sh_peer pe;
	struct sched_param sp;
	char msg[200];
	int lo, hi, r, saved;

	pc_begin("XSH.sched_getparam.004");
	if (sh_rng(SCHED_FIFO, &lo, &hi, msg) != SH_PASS)
		pc_unresolved("%s", msg);
	sh_peer_start(&pe, SCHED_FIFO, lo + 2, -1, 0);
	memset(&sp, 0, sizeof sp);
	sp.sched_priority = INT_MIN;
	pc_test();
	errno = 0;
	r = sched_getparam(pe.pid, &sp);
	saved = errno;
	(void)sh_peer_finish(&pe, msg);
	if (r == -1 && saved == EPERM)
		pc_unresolved("permission to query the peer was refused although privileged");
	pc_check(r == 0, "sched_getparam(peer) failed: %s", pc_errname(saved));
	pc_check(sp.sched_priority == lo + 2, "peer has priority %d but %d was returned", lo + 2, sp.sched_priority);
	pc_pass();
}
