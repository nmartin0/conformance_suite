/*
 * getnetbyname() finds the first entry of the database for the name of that entry.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getnetent.003
 */
#include "dbcommon.h"

int main(void)
{
	const struct netent *e;
	char first[SIGMAX], name[SIGMAX], buf[SIGMAX];

	pc_begin("XSH.getnetent.003");
	pc_test();
	setnetent(0);
	e = getnetent();
	if (e == NULL)
		pc_notinuse("the database has no entry");
	sig_of(e, first, sizeof first);
	snprintf(name, sizeof name, "%s", e->n_name);
	e = getnetbyname(name);
	pc_check(e != NULL, "getnetbyname(\"%s\") found nothing", name);
	sig_of(e, buf, sizeof buf);
	pc_check(strcmp(buf, first) == 0, "getnetbyname(\"%s\") returned \"%s\", the first entry is \"%s\"", name, buf, first);
	pc_pass();
}
