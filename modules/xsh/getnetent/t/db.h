/*
 * db.h - the netent binding for dbcommon.h.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef DB_H
#define DB_H
#include <stdint.h>
#define DBT struct netent
#define GETENT() getnetent()
#define SETENT(n) setnetent(n)
#define ENDENT() endnetent()

#define S(x) ((x) != NULL ? (x) : "(null)")

static void sig_of(const struct netent *e, char *buf, size_t n)
{
	snprintf(buf, n, "%s|%d|%lu", S(e->n_name), e->n_addrtype, (unsigned long)e->n_net);
}

static void shape(const struct netent *e)
{
	int i;

	pc_check(e->n_name != NULL, "an entry has a null n_name");
	pc_check(e->n_aliases != NULL, "an entry has a null n_aliases");
	for (i = 0; e->n_aliases[i] != NULL; i++)
		pc_check(i < 1000, "n_aliases is not terminated by a null pointer");
}
#endif
