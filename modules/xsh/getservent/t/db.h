/*
 * db.h - the servent binding for dbcommon.h.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef DB_H
#define DB_H
#include <stdint.h>
#define DBT struct servent
#define GETENT() getservent()
#define SETENT(n) setservent(n)
#define ENDENT() endservent()

#define S(x) ((x) != NULL ? (x) : "(null)")

static void sig_of(const struct servent *e, char *buf, size_t n)
{
	snprintf(buf, n, "%s|%d|%s", S(e->s_name), (int)(uint16_t)e->s_port, S(e->s_proto));
}

static void shape(const struct servent *e)
{
	int i;

	pc_check(e->s_name != NULL, "an entry has a null s_name");
	pc_check(e->s_proto != NULL, "an entry has a null s_proto");
	pc_check(e->s_aliases != NULL, "an entry has a null s_aliases");
	for (i = 0; e->s_aliases[i] != NULL; i++)
		pc_check(i < 1000, "s_aliases is not terminated by a null pointer");
}
#endif
