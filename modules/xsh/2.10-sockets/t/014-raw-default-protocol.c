/*
 * A raw socket made with protocol 0 uses IPPROTO_RAW, and IPPROTO_ICMP is
 * available as a raw protocol.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.10-sockets.014
 */
#include "pcsk.h"

int main(void)
{
	int s, v, i;
	socklen_t l;

	pc_begin("XSH.2.10-sockets.014");
	guard(60);
	pc_test();
	s = socket(AF_INET, SOCK_RAW, 0);
	if (s == -1 && (errno == EACCES || errno == EPERM))
		pc_unresolved("the process lacks the privileges to make a raw socket (%s)", pc_errname(errno));
	pc_check(s != -1, "socket(AF_INET, SOCK_RAW, 0) failed: %s", pc_errname(errno));
	v = -1;
	l = sizeof v;
	pc_check(getsockopt(s, SOL_SOCKET, SO_PROTOCOL, &v, &l) == 0, "getsockopt(SO_PROTOCOL) failed: %s", pc_errname(errno));
	pc_check(v == IPPROTO_RAW, "the protocol of a raw socket made with protocol 0 is %d, expected IPPROTO_RAW", v);
	v = -1;
	l = sizeof v;
	pc_check(getsockopt(s, SOL_SOCKET, SO_TYPE, &v, &l) == 0, "getsockopt(SO_TYPE) failed: %s", pc_errname(errno));
	pc_check(v == SOCK_RAW, "the type of the raw socket is %d, expected SOCK_RAW", v);
	(void)close(s);
	i = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	pc_check(i != -1, "socket(AF_INET, SOCK_RAW, IPPROTO_ICMP) failed: %s", pc_errname(errno));
	v = -1;
	l = sizeof v;
	pc_check(getsockopt(i, SOL_SOCKET, SO_PROTOCOL, &v, &l) == 0, "getsockopt(SO_PROTOCOL) failed: %s", pc_errname(errno));
	pc_check(v == IPPROTO_ICMP, "the protocol of the ICMP raw socket is %d, expected IPPROTO_ICMP", v);
	(void)close(i);
	pc_pass();
}
