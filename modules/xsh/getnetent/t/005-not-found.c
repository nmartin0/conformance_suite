/*
 * getnetbyname() of a name no network has, and getnetbyaddr() with an address type no entry has, return a null pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getnetent.005
 */
#include "dbcommon.h"

int main(void)
{
	pc_begin("XSH.getnetent.005");
	pc_test();
	/* Control: a database with an entry shows the lookups work at all; with none, a null answer proves nothing. */
	collect(0, 0, 0);
	if (seqn[0] == 0)
		pc_notinuse("the database has no entry");
	pc_check(getnetbyname("pc-no-such-network-zz") == NULL, "getnetbyname() of an unknown name returned an entry");
	pc_check(getnetbyaddr(1, -1) == NULL, "getnetbyaddr() with address type -1 returned an entry");
	pc_pass();
}
