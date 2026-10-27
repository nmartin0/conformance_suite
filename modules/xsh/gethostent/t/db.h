/*
 * db.h - the hostent binding for dbcommon.h.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef DB_H
#define DB_H
#include <stdint.h>
#define DBT struct hostent
#define GETENT() gethostent()
#define SETENT(n) sethostent(n)
#define ENDENT() endhostent()

#define S(x) ((x) != NULL ? (x) : "(null)")

static void sig_of(const struct hostent *e, char *buf, size_t n)
{
	size_t used;
	int i, j;

	used = (size_t)snprintf(buf, n, "%s|%d|%d", S(e->h_name), e->h_addrtype, e->h_length);
	for (i = 0; e->h_addr_list != NULL && e->h_addr_list[i] != NULL && i < 64; i++)
		for (j = 0; j < e->h_length && j < 16 && used + 4 < n; j++)
			used += (size_t)snprintf(buf + used, n - used, "%s%02x", j == 0 ? "|" : "", (unsigned char)e->h_addr_list[i][j]);
}

static void shape(const struct hostent *e)
{
	int i;

	pc_check(e->h_name != NULL, "an entry has a null h_name");
	pc_check(e->h_aliases != NULL, "an entry has a null h_aliases");
	for (i = 0; e->h_aliases[i] != NULL; i++)
		pc_check(i < 1000, "h_aliases is not terminated by a null pointer");
	pc_check(e->h_addr_list != NULL, "an entry has a null h_addr_list");
	for (i = 0; e->h_addr_list[i] != NULL; i++)
		pc_check(i < 1000, "h_addr_list is not terminated by a null pointer");
	if (e->h_addrtype == AF_INET)
		pc_check(e->h_length == 4, "an AF_INET entry has h_length %d", e->h_length);
	if (e->h_addrtype == AF_INET6)
		pc_check(e->h_length == 16, "an AF_INET6 entry has h_length %d", e->h_length);
}
#endif
