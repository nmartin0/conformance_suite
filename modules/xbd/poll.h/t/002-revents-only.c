/*
 * POLLERR, POLLHUP and POLLNVAL each share no bits with any other POLL
 * constant.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.poll.h.002
 */
#include "pc.h"
#include <poll.h>

int main(void)
{
	pc_begin("XBD.poll.h.002");
#if !defined(POLLIN) || !defined(POLLRDNORM) || !defined(POLLRDBAND) || !defined(POLLPRI) || !defined(POLLOUT) || !defined(POLLWRNORM) || !defined(POLLWRBAND) || !defined(POLLERR) || !defined(POLLHUP) || !defined(POLLNVAL)
	pc_unresolved("a POLL constant is missing (the header check reports it)");
#else
	static const char *const nm[10] = { "POLLIN", "POLLRDNORM", "POLLRDBAND", "POLLPRI", "POLLOUT", "POLLWRNORM", "POLLWRBAND", "POLLERR", "POLLHUP", "POLLNVAL" };
	unsigned long long v[10];
	int i, j;

	v[0] = (unsigned long long)(POLLIN);
	v[1] = (unsigned long long)(POLLRDNORM);
	v[2] = (unsigned long long)(POLLRDBAND);
	v[3] = (unsigned long long)(POLLPRI);
	v[4] = (unsigned long long)(POLLOUT);
	v[5] = (unsigned long long)(POLLWRNORM);
	v[6] = (unsigned long long)(POLLWRBAND);
	v[7] = (unsigned long long)(POLLERR);
	v[8] = (unsigned long long)(POLLHUP);
	v[9] = (unsigned long long)(POLLNVAL);
	pc_test();
	/* POLLERR, POLLHUP and POLLNVAL are reported without being asked for, beside the requested bits. */
	for (i = 7; i < 10; i++)
		for (j = 0; j < 10; j++)
			if (j != i)
				pc_check((v[i] & v[j]) == 0, "%s (%#llx) and %s (%#llx) have bits in common", nm[i], v[i], nm[j], v[j]);
	pc_pass();
#endif
}
