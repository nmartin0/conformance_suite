/*
 * Entries found by getprotobyname() and getprotobynumber() for a later entry of the database carry the name or number asked for.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getprotoent.004
 */
#include "dbcommon.h"

#include <strings.h>

int main(void)
{
	const struct protoent *e;
	char name[SIGMAX];
	int num = 0, k = 0, last = 0;

	pc_begin("XSH.getprotoent.004");
	pc_test();
	setprotoent(0);
	while ((e = getprotoent()) != NULL) {
		k++;
		last = k;
		snprintf(name, sizeof name, "%s", e->p_name);
		num = e->p_proto;
		if (k >= 1000)
			break;
	}
	if (last == 0)
		pc_notinuse("the database has no entry");
	e = getprotobynumber(num);
	pc_check(e != NULL, "getprotobynumber(%d) found nothing", num);
	pc_check(e->p_proto == num, "getprotobynumber(%d) returned an entry numbered %d", num, e->p_proto);
	e = getprotobyname(name);
	pc_check(e != NULL, "getprotobyname(\"%s\") found nothing", name);
	pc_check(strcasecmp(e->p_name, name) == 0, "getprotobyname(\"%s\") returned an entry named \"%s\"", name, e->p_name);
	pc_pass();
}
