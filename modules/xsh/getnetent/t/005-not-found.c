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
	pc_check(getnetbyname("pc-no-such-network-zz") == NULL, "getnetbyname() of an unknown name returned an entry");
	pc_check(getnetbyaddr(1, -1) == NULL, "getnetbyaddr() with address type -1 returned an entry");
	pc_pass();
}
