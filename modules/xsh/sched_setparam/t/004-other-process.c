/*
 * sched_setparam() changes the priority of another process.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.sched_setparam.004
 */
#include "schedh.h"

int main(void)
{
	struct sh_peer pe;
	struct sched_param sp;
	char msg[200];
	int lo, hi, r, saved;

	pc_begin("XSH.sched_setparam.004");
	if (sh_rng(SCHED_FIFO, &lo, &hi, msg) != SH_PASS)
		pc_unresolved("%s", msg);
	sh_peer_start(&pe, SCHED_FIFO, lo, SCHED_FIFO, lo + 2);
	memset(&sp, 0, sizeof sp);
	sp.sched_priority = lo + 2;
	pc_test();
	errno = 0;
	r = sched_setparam(pe.pid, &sp);
	saved = errno;
	if (r == -1 && saved == EPERM) {
		(void)sh_peer_finish(&pe, msg);
		pc_unresolved("permission to change the peer was refused although privileged");
	}
	if (r != 0) {
		(void)sh_peer_finish(&pe, msg);
		pc_fail("sched_setparam(peer) failed: %s", pc_errname(saved));
	}
	if (sh_peer_finish(&pe, msg) != SH_PASS)
		pc_fail("%s", msg);
	pc_pass();
}
