/*
 * getprotobyname() of a name no protocol has and getprotobynumber() of a number no protocol has return a null pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getprotoent.005
 */
#include "dbcommon.h"

int main(void)
{
	pc_begin("XSH.getprotoent.005");
	pc_test();
	/* Control: a database with an entry shows the lookups work at all; with none, a null answer proves nothing. */
	collect(0, 0, 0);
	if (seqn[0] == 0)
		pc_notinuse("the database has no entry");
	pc_check(getprotobyname("pc-no-such-protocol-zz") == NULL, "getprotobyname() of an unknown name returned an entry");
	pc_check(getprotobynumber(12345678) == NULL, "getprotobynumber() of 12345678 returned an entry");
	pc_pass();
}
