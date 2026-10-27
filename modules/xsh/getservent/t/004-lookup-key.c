/*
 * getservbyport() finds the first entry of the database for the port and protocol of that entry, and with a null protocol too.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getservent.004
 */
#include "dbcommon.h"

int main(void)
{
	const struct servent *e;
	char first[SIGMAX], proto[SIGMAX], buf[SIGMAX];
	int port;

	pc_begin("XSH.getservent.004");
	pc_test();
	setservent(0);
	e = getservent();
	if (e == NULL)
		pc_notinuse("the database has no entry");
	sig_of(e, first, sizeof first);
	port = (int)(uint16_t)e->s_port;
	snprintf(proto, sizeof proto, "%s", e->s_proto);
	e = getservbyport(port, proto);
	pc_check(e != NULL, "getservbyport(%d, \"%s\") found nothing", port, proto);
	sig_of(e, buf, sizeof buf);
	pc_check(strcmp(buf, first) == 0, "getservbyport(%d, \"%s\") returned \"%s\", the first entry is \"%s\"", port, proto, buf, first);
	e = getservbyport(port, NULL);
	pc_check(e != NULL, "getservbyport(%d, NULL) found nothing", port);
	sig_of(e, buf, sizeof buf);
	pc_check(strcmp(buf, first) == 0, "getservbyport(%d, NULL) returned \"%s\", the first entry is \"%s\"", port, buf, first);
	pc_pass();
}
