/*
 * inet_pton() with an unknown address family returns -1 and sets EAFNOSUPPORT.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.inet_ntop.007
 */
#include "ip.h"

int main(void)
{
	static const int afs[] = { AF_UNSPEC, -1 };
	size_t i;

	pc_begin("XSH.inet_ntop.007");
	pc_test();
	for (i = 0; i < 2; i++) {
		unsigned char out[16];
		int rc;

		errno = 0;
		rc = inet_pton(afs[i], "1.2.3.4", out);
		pc_check(rc == -1 && errno == EAFNOSUPPORT, "inet_pton(%d) returned %d, errno %s; expected -1 and EAFNOSUPPORT", afs[i], rc, pc_errname(errno));
	}
	pc_pass();
}
