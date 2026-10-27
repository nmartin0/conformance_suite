/*
 * inet_ntop() with an unknown address family returns a null pointer and sets EAFNOSUPPORT.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.inet_ntop.013
 */
#include "ip.h"

int main(void)
{
	static const int afs[] = { AF_UNSPEC, -1 };
	static const unsigned char a[16] = { 1, 2, 3, 4 };
	size_t i;

	pc_begin("XSH.inet_ntop.013");
	pc_test();
	for (i = 0; i < 2; i++) {
		char buf[INET6_ADDRSTRLEN];
		const char *r;

		errno = 0;
		r = inet_ntop(afs[i], a, buf, sizeof buf);
		pc_check(r == NULL && errno == EAFNOSUPPORT, "family %d gave %s, errno %s; expected NULL and EAFNOSUPPORT", afs[i], r ? "a pointer" : "NULL", pc_errname(errno));
	}
	pc_pass();
}
