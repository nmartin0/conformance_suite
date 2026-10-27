/*
 * poll() on the write end of an empty pipe, asked for POLLWRNORM and asked
 * for POLLOUT, reports a writable descriptor with POLLWRNORM or POLLOUT
 * set.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.poll.h.004
 */
#include "pc.h"
#include <errno.h>
#include <poll.h>
#include <unistd.h>

int main(void)
{
	int p[2];
	struct pollfd f;
	int r;

	pc_begin("XBD.poll.h.004");
#if !defined(POLLWRNORM) || !defined(POLLOUT)
	pc_unresolved("POLLWRNORM or POLLOUT is missing (the header check reports it)");
#else
	if (pipe(p) == -1)
		pc_unresolved("pipe failed (%s)", pc_errname(errno));
	pc_test();
	/* The write end of an empty pipe is ready for writing. */
	f.fd = p[1];
	f.events = POLLWRNORM;
	f.revents = 0;
	r = poll(&f, 1, 0);
	pc_check(r == 1, "poll with POLLWRNORM on the write end of an empty pipe returned %d (%s)", r, r == -1 ? pc_errname(errno) : "");
	pc_check((f.revents & (POLLWRNORM | POLLOUT)) != 0, "poll with POLLWRNORM reported neither POLLWRNORM nor POLLOUT (revents %#x)", (unsigned)f.revents);
	f.fd = p[1];
	f.events = POLLOUT;
	f.revents = 0;
	r = poll(&f, 1, 0);
	pc_check(r == 1, "poll with POLLOUT on the write end of an empty pipe returned %d (%s)", r, r == -1 ? pc_errname(errno) : "");
	pc_check((f.revents & (POLLWRNORM | POLLOUT)) != 0, "poll with POLLOUT reported neither POLLWRNORM nor POLLOUT (revents %#x)", (unsigned)f.revents);
	close(p[0]);
	close(p[1]);
	pc_pass();
#endif
}
