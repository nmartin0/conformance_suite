/*
 * getprotobyname() finds the first entry of the database for the name of that entry, and getprotobynumber() the first entry for its number.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getprotoent.003
 */
#include "dbcommon.h"

int main(void)
{
	const struct protoent *e;
	char first[SIGMAX], name[SIGMAX], buf[SIGMAX];
	int num;

	pc_begin("XSH.getprotoent.003");
	pc_test();
	setprotoent(0);
	e = getprotoent();
	if (e == NULL)
		pc_notinuse("the database has no entry");
	sig_of(e, first, sizeof first);
	snprintf(name, sizeof name, "%s", e->p_name);
	num = e->p_proto;
	e = getprotobyname(name);
	pc_check(e != NULL, "getprotobyname(\"%s\") found nothing", name);
	sig_of(e, buf, sizeof buf);
	pc_check(strcmp(buf, first) == 0, "getprotobyname(\"%s\") returned \"%s\", the first entry is \"%s\"", name, buf, first);
	e = getprotobynumber(num);
	pc_check(e != NULL, "getprotobynumber(%d) found nothing", num);
	sig_of(e, buf, sizeof buf);
	pc_check(strcmp(buf, first) == 0, "getprotobynumber(%d) returned \"%s\", the first entry is \"%s\"", num, buf, first);
	pc_pass();
}
