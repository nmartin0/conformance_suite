/*
 * getservbyname() finds the first entry of the database for the name and protocol of that entry, and with a null protocol too.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getservent.003
 */
#include "dbcommon.h"

int main(void)
{
	const struct servent *e;
	char first[SIGMAX], name[SIGMAX], proto[SIGMAX], buf[SIGMAX];

	pc_begin("XSH.getservent.003");
	pc_test();
	setservent(0);
	e = getservent();
	if (e == NULL)
		pc_notinuse("the database has no entry");
	sig_of(e, first, sizeof first);
	snprintf(name, sizeof name, "%s", e->s_name);
	snprintf(proto, sizeof proto, "%s", e->s_proto);
	e = getservbyname(name, proto);
	pc_check(e != NULL, "getservbyname(\"%s\", \"%s\") found nothing", name, proto);
	sig_of(e, buf, sizeof buf);
	pc_check(strcmp(buf, first) == 0, "getservbyname(\"%s\", \"%s\") returned \"%s\", the first entry is \"%s\"", name, proto, buf, first);
	e = getservbyname(name, NULL);
	pc_check(e != NULL, "getservbyname(\"%s\", NULL) found nothing", name);
	sig_of(e, buf, sizeof buf);
	pc_check(strcmp(buf, first) == 0, "getservbyname(\"%s\", NULL) returned \"%s\", the first entry is \"%s\"", name, buf, first);
	pc_pass();
}
