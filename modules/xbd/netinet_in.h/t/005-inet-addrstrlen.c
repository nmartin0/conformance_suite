/*
 * INET_ADDRSTRLEN is 16.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.netinet_in.h.005
 */
#include <netinet/in.h>
#include "pc.h"


#ifdef INET_ADDRSTRLEN
static char buf[INET_ADDRSTRLEN];
enum { len = INET_ADDRSTRLEN };
#endif

int main(void)
{
	pc_begin("XBD.netinet_in.h.005");
	pc_test();
#ifndef INET_ADDRSTRLEN
	pc_fail("<netinet/in.h> does not define INET_ADDRSTRLEN");
#else
	pc_check(len == 16 && sizeof buf == 16, "INET_ADDRSTRLEN is %d, not 16", (int)len);
	pc_pass();
#endif
}
