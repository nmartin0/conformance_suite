/*
 * A lookup matches the whole ID: a neighbouring ID finds only an entry with exactly that ID.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getpwuid.007
 */
#include "pwdb.h"

int main(void)
{
	struct xc c;
	xid_t q[2];
	char buf[16384];
	xent_t e, *res, *p;
	int i, r;

	pc_begin("XSH.getpwuid.007");
	xc_find(&c);
	q[0] = (xid_t)(c.id + 1);
	q[1] = (xid_t)(c.id - 1);
	pc_check(getpwuid(c.id) != NULL, "no entry found for ID %ld", (long)c.id);
	pc_test();
	for (i = 0; i < 2; i++) {
		p = getpwuid(q[i]);
		pc_check(p == NULL || XID(p) == q[i], "ID %ld found the entry with ID %ld",
		    (long)q[i], (long)(p ? XID(p) : 0));
		memset(&e, 0, sizeof e);
		res = NULL;
		r = getpwuid_r(q[i], &e, buf, sizeof buf, &res);
		pc_check(r != 0 || res == NULL || XID(res) == q[i],
		    "the reentrant lookup of ID %ld found the entry with ID %ld", (long)q[i], (long)(res != NULL ? XID(res) : 0));
	}
	pc_pass();
}
