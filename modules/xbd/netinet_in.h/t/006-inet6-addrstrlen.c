/*
 * INET6_ADDRSTRLEN is 46.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.netinet_in.h.006
 */
#include <netinet/in.h>
#include "pc.h"


#ifdef INET6_ADDRSTRLEN
static char buf[INET6_ADDRSTRLEN];
enum { len = INET6_ADDRSTRLEN };
#endif

int main(void)
{
	pc_begin("XBD.netinet_in.h.006");
	pc_test();
#ifndef INET6_ADDRSTRLEN
	pc_fail("<netinet/in.h> does not define INET6_ADDRSTRLEN");
#else
	pc_check(len == 46 && sizeof buf == 46, "INET6_ADDRSTRLEN is %d, not 46", (int)len);
	pc_pass();
#endif
}
