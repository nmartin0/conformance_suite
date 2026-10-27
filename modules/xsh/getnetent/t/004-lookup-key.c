/*
 * getnetbyaddr() finds the first entry of the database for the address type and network number (host byte order) of that entry.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getnetent.004
 */
#include "dbcommon.h"

int main(void)
{
	const struct netent *e;
	char first[SIGMAX], buf[SIGMAX];
	uint32_t net;
	int type;

	pc_begin("XSH.getnetent.004");
	pc_test();
	setnetent(0);
	e = getnetent();
	if (e == NULL)
		pc_notinuse("the database has no entry");
	sig_of(e, first, sizeof first);
	net = e->n_net;
	type = e->n_addrtype;
	e = getnetbyaddr(net, type);
	pc_check(e != NULL, "getnetbyaddr(%lu, %d) found nothing", (unsigned long)net, type);
	sig_of(e, buf, sizeof buf);
	pc_check(strcmp(buf, first) == 0, "getnetbyaddr(%lu, %d) returned \"%s\", the first entry is \"%s\"", (unsigned long)net, type, buf, first);
	pc_pass();
}
