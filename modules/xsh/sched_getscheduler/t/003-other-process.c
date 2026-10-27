/*
 * sched_getscheduler() of another process returns that process's policy.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_getscheduler.003
 */
#include "schedh.h"

int main(void)
{
	struct sh_peer pe;
	char msg[200];
	int lo, hi, r, saved;

	pc_begin("XSH.sched_getscheduler.003");
	if (sh_rng(SCHED_FIFO, &lo, &hi, msg) != SH_PASS)
		pc_unresolved("%s", msg);
	sh_peer_start(&pe, SCHED_FIFO, lo + 1, -1, 0);
	pc_test();
	errno = 0;
	r = sched_getscheduler(pe.pid);
	saved = errno;
	if (r == -1 && saved == EPERM) {
		(void)sh_peer_finish(&pe, msg);
		pc_unresolved("permission to query the peer was refused although privileged");
	}
	(void)sh_peer_finish(&pe, msg);
	pc_check(r != -1, "sched_getscheduler(peer) failed: %s", pc_errname(saved));
	pc_check(r == SCHED_FIFO, "peer runs SCHED_FIFO (%d) but %d was returned", SCHED_FIFO, r);
	pc_pass();
}
