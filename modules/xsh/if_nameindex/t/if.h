/*
 * if.h - helpers shared by the interface-name tests.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef IF_H
#define IF_H
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The interface list, or UNRESOLVED (the call may fail for lack of resources). */
static struct if_nameindex *get_list(void)
{
	struct if_nameindex *l = if_nameindex();

	if (l == NULL)
		pc_unresolved("if_nameindex() failed: %s", pc_errname(errno));
	return l;
}

static int list_len(const struct if_nameindex *l)
{
	int n = 0;

	while (l[n].if_index != 0 || l[n].if_name != NULL)
		n++;
	return n;
}

static int in_list(const struct if_nameindex *l, const char *name)
{
	int i;

	for (i = 0; l[i].if_index != 0 || l[i].if_name != NULL; i++)
		if (l[i].if_name != NULL && strcmp(l[i].if_name, name) == 0)
			return 1;
	return 0;
}
#endif
