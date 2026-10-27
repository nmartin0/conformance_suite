/*
 * db.h - the protoent binding for dbcommon.h.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef DB_H
#define DB_H
#include <stdint.h>
#define DBT struct protoent
#define GETENT() getprotoent()
#define SETENT(n) setprotoent(n)
#define ENDENT() endprotoent()

#define S(x) ((x) != NULL ? (x) : "(null)")

static void sig_of(const struct protoent *e, char *buf, size_t n)
{
	snprintf(buf, n, "%s|%d", S(e->p_name), e->p_proto);
}

static void shape(const struct protoent *e)
{
	int i;

	pc_check(e->p_name != NULL, "an entry has a null p_name");
	pc_check(e->p_aliases != NULL, "an entry has a null p_aliases");
	for (i = 0; e->p_aliases[i] != NULL; i++)
		pc_check(i < 1000, "p_aliases is not terminated by a null pointer");
}
#endif
