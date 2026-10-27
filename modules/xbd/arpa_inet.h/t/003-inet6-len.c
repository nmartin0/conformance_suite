/*
 * <arpa/inet.h> defines INET6_ADDRSTRLEN as 46.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.arpa_inet.h.003
 */
#include <arpa/inet.h>
#include "pc.h"


int main(void)
{
	pc_begin("XBD.arpa_inet.h.003");
	pc_test();
#ifndef INET6_ADDRSTRLEN
	pc_fail("<arpa/inet.h> does not define INET6_ADDRSTRLEN");
#else
	pc_check(INET6_ADDRSTRLEN == 46, "INET6_ADDRSTRLEN is not 46");
	pc_pass();
#endif
}
