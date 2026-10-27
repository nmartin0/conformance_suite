/*
 * A null hints pointer acts as a zeroed hints structure, and every entry can make a socket.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getaddrinfo.004
 */
#include "ga.h"

int main(void)
{
	struct addrinfo z = mkhints(0, AF_UNSPEC, 0, 0), *a = NULL, *b = NULL, *p, *q;
	int rc;

	pc_begin("XSH.getaddrinfo.004");
	pc_test();
	rc = getaddrinfo("127.0.0.1", "80", NULL, &a);
	pc_check(rc == 0 && a != NULL, "getaddrinfo with null hints failed: %s", gai_strerror(rc));
	rc = getaddrinfo("127.0.0.1", "80", &z, &b);
	pc_check(rc == 0 && b != NULL, "getaddrinfo with zeroed hints failed: %s", gai_strerror(rc));
	pc_check(count(a) == count(b), "null hints gave %d entries, zeroed hints %d", count(a), count(b));
	for (p = a, q = b; p != NULL && q != NULL; p = p->ai_next, q = q->ai_next) {
		pc_check(p->ai_family == q->ai_family && p->ai_socktype == q->ai_socktype && p->ai_protocol == q->ai_protocol,
		    "entries differ in family, type or protocol");
		pc_check(p->ai_addrlen == q->ai_addrlen && memcmp(p->ai_addr, q->ai_addr, p->ai_addrlen) == 0,
		    "entries differ in the socket address");
		socketable(p, -1);
	}
	freeaddrinfo(a);
	freeaddrinfo(b);
	pc_pass();
}
