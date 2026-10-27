/*
 * IPPORT_RESERVED is a port number.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.netdb.h.004
 */
#include <netdb.h>
#include "pc.h"


#ifdef IPPORT_RESERVED
static const long value = IPPORT_RESERVED;
#endif

int main(void)
{
	pc_begin("XBD.netdb.h.004");
	pc_test();
#ifndef IPPORT_RESERVED
	pc_fail("<netdb.h> does not define IPPORT_RESERVED");
#else
	pc_check(value >= 0 && value <= 65535, "IPPORT_RESERVED is %ld, not a port number", value);
	pc_pass();
#endif
}
