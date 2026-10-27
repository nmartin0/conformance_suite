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
	pc_check(getservbyname("pc-no-such-service-zz", NULL) == NULL, "getservbyname() of an unknown name returned an entry");
	pc_check(getservbyname("pc-no-such-service-zz", "pc-no-such-proto-zz") == NULL, "getservbyname() of an unknown name and protocol returned an entry");
	pc_check(getservbyport((int)(uint16_t)htons(80), "pc-no-such-proto-zz") == NULL, "getservbyport() with an unknown protocol returned an entry");
	pc_pass();
}
