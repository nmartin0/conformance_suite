/*
 * getgroups.c - conforming variants of getgroups(), for the variant
 * checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <errno.h>
#include <unistd.h>
#include "pcvariant.h"

/*
 * "with_egid": the effective group ID is reported first, before the
 * supplementary groups. XSH getgroups() DESCRIPTION leaves it to the
 * implementation whether the effective group ID is returned; the count
 * then includes it, and a size below that count is EINVAL as usual.
 * "reversed": the groups come in reverse order; the page fixes no order.
 * "scribbles": entries at and after the returned count are overwritten;
 * their values are undefined (DESCRIPTION).
 */
int getgroups(int size, gid_t list[])
{
	const char *v = variant_of("getgroups.");
	gid_t tmp[1024];
	int n, i, total;
	REAL(getgroups);

	if (v == NULL)
		return real_getgroups(size, list);
	n = real_getgroups(1024, tmp);
	if (n < 0)
		return n;
	if (IS("with_egid")) {
		for (i = n; i > 0; i--)
			tmp[i] = tmp[i - 1];
		tmp[0] = getegid();
		n++;
	}
	total = n;
	if (size == 0)
		return total;
	if (size < total) {
		errno = EINVAL;
		return -1;
	}
	for (i = 0; i < n; i++)
		list[i] = IS("reversed") ? tmp[n - 1 - i] : tmp[i];
	if (IS("scribbles"))
		for (i = n; i < size && i < n + 16; i++)
			list[i] = (gid_t)0xdead;
	return total;
}
