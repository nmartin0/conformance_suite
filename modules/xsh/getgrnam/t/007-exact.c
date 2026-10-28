/*
 * A lookup matches the whole name: a longer or shorter name finds only an entry with exactly that name.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getgrnam.007
 */
#include "grdb.h"

int main(void)
{
	struct xc c;
	char q[3][600], buf[16384];
	xent_t e, *res, *p;
	size_t len;
	int i, r;

	pc_begin("XSH.getgrnam.007");
	xc_find(&c);
	len = strlen(c.name);
	snprintf(q[0], sizeof q[0], "%sx", c.name);
	snprintf(q[1], sizeof q[1], "x%s", c.name);
	snprintf(q[2], sizeof q[2], "%.*s", (int)(len > 1 ? len - 1 : 0), c.name);
	pc_check(getgrnam(c.name) != NULL, "no entry found for %s", c.name);
	pc_test();
	/* control: the lookup works, so a null result for a changed name means something */
	errno = 0;
	p = getgrnam(c.name);
	if (p == NULL && xc_mayfail(errno))
		pc_unresolved("getgrnam() of an existing name failed: %s", pc_errname(errno));
	pc_check(p != NULL && XNAME(p) != NULL && strcmp(XNAME(p), c.name) == 0, "getgrnam() of the existing name %s did not find its entry", c.name);
	memset(&e, 0, sizeof e);
	res = NULL;
	r = getgrnam_r(c.name, &e, buf, sizeof buf, &res);
	if (r != 0 && xc_mayfail(r))
		pc_unresolved("getgrnam_r() of an existing name failed: %s", pc_errname(r));
	pc_check(r == 0 && res != NULL && XNAME(res) != NULL && strcmp(XNAME(res), c.name) == 0, "getgrnam_r() of the existing name %s did not find its entry", c.name);
	for (i = 0; i < 3; i++) {
		if (q[i][0] == '\0')
			continue;
		p = getgrnam(q[i]);
		pc_check(p == NULL || (XNAME(p) != NULL && strcmp(XNAME(p), q[i]) == 0),
		    "the name %s found the entry of %s", q[i], p != NULL && XNAME(p) != NULL ? XNAME(p) : "");
		memset(&e, 0, sizeof e);
		res = NULL;
		r = getgrnam_r(q[i], &e, buf, sizeof buf, &res);
		pc_check(r != 0 || res == NULL || (XNAME(res) != NULL && strcmp(XNAME(res), q[i]) == 0),
		    "the reentrant lookup of %s found the entry of %s", q[i], res != NULL && XNAME(res) != NULL ? XNAME(res) : "");
	}
	pc_pass();
}
