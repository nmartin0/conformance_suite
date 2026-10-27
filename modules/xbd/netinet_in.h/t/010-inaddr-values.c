/*
 * INADDR_ANY is 0 and INADDR_BROADCAST is all ones, whatever the byte
 *   order.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.netinet_in.h.010
 */
#include <netinet/in.h>
#include <arpa/inet.h>
#include "pc.h"


int main(void)
{
	in_addr_t any, bcast;

	pc_begin("XBD.netinet_in.h.010");
	pc_test();
#if !defined(INADDR_ANY) || !defined(INADDR_BROADCAST)
	pc_fail("<netinet/in.h> does not define INADDR_ANY and INADDR_BROADCAST");
#else
	any = INADDR_ANY;
	bcast = INADDR_BROADCAST;
	pc_check(any == 0, "INADDR_ANY is %#lx, not 0", (unsigned long)any);
	pc_check(bcast == 0xffffffffu, "INADDR_BROADCAST is %#lx, not all ones", (unsigned long)bcast);
	pc_check(htonl(any) == any && htonl(bcast) == bcast, "INADDR_ANY or INADDR_BROADCAST changes under htonl()");
	pc_pass();
#endif
}
