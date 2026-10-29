/*
 * getservbyname() of a name no service has, and getservbyport() or getservbyname() with a protocol no entry has, return a null pointer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getservent.005
 */
#include "dbcommon.h"

int main(void)
{
	pc_begin("XSH.getservent.005");
	pc_test();
	/* Control: a database with an entry shows the lookups work at all; with none, a null answer proves nothing. */
	collect(0, 0, 0);
	if (seqn[0] == 0)
		pc_notinuse("the database has no entry");
	pc_check(getservbyname("pc-no-such-service-zz", NULL) == NULL, "getservbyname() of an unknown name returned an entry");
	pc_check(getservbyname("pc-no-such-service-zz", "pc-no-such-proto-zz") == NULL, "getservbyname() of an unknown name and protocol returned an entry");
	pc_check(getservbyport((int)(uint16_t)htons(80), "pc-no-such-proto-zz") == NULL, "getservbyport() with an unknown protocol returned an entry");
	pc_pass();
}
