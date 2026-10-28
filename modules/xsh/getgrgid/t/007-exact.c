/*
 * A lookup matches the whole ID: a neighbouring ID finds only an entry with exactly that ID.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getgrgid.007
 */
#include "grdb.h"

int main(void)
{
	struct xc c;
	xid_t q[2];
	char buf[16384];
	xent_t e, *res, *p;
	int i, r;

	pc_begin("XSH.getgrgid.007");
	xc_find(&c);
	q[0] = (xid_t)(c.id + 1);
	q[1] = (xid_t)(c.id - 1);
	pc_check(getgrgid(c.id) != NULL, "no entry found for ID %ld", (long)c.id);
	pc_test();
	/* control: the lookup works, so a null result for a neighbour means something */
	errno = 0;
	p = getgrgid(c.id);
	if (p == NULL && xc_mayfail(errno))
		pc_unresolved("getgrgid() of an existing ID failed: %s", pc_errname(errno));
	pc_check(p != NULL && XID(p) == c.id, "getgrgid() of the existing ID %ld did not find its entry", (long)c.id);
	memset(&e, 0, sizeof e);
	res = NULL;
	r = getgrgid_r(c.id, &e, buf, sizeof buf, &res);
	if (r != 0 && xc_mayfail(r))
		pc_unresolved("getgrgid_r() of an existing ID failed: %s", pc_errname(r));
	pc_check(r == 0 && res != NULL && XID(res) == c.id, "getgrgid_r() of the existing ID %ld did not find its entry", (long)c.id);
	for (i = 0; i < 2; i++) {
		p = getgrgid(q[i]);
		pc_check(p == NULL || XID(p) == q[i], "ID %ld found the entry with ID %ld",
		    (long)q[i], (long)(p ? XID(p) : 0));
		memset(&e, 0, sizeof e);
		res = NULL;
		r = getgrgid_r(q[i], &e, buf, sizeof buf, &res);
		pc_check(r != 0 || res == NULL || XID(res) == q[i],
		    "the reentrant lookup of ID %ld found the entry with ID %ld", (long)q[i], (long)(res != NULL ? XID(res) : 0));
	}
	pc_pass();
}
