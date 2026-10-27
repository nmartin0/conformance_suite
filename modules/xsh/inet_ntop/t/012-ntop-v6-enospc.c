/*
 * inet_ntop() with a too small IPv6 buffer returns a null pointer and sets ENOSPC.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.inet_ntop.012
 */
#include "ip.h"

int main(void)
{
	static const unsigned char ones[16] = { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 };
	static const unsigned char loop[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };
	char buf[INET6_ADDRSTRLEN];
	const char *r;

	pc_begin("XSH.inet_ntop.012");
	pc_test();
	errno = 0;
	r = inet_ntop(AF_INET6, loop, buf, 0);
	pc_check(r == NULL && errno == ENOSPC, "size 0 gave %s, errno %s", r ? "a pointer" : "NULL", pc_errname(errno));
	errno = 0;
	r = inet_ntop(AF_INET6, loop, buf, 2);
	pc_check(r == NULL && errno == ENOSPC, "size 2 gave %s, errno %s", r ? "a pointer" : "NULL", pc_errname(errno));
	errno = 0;
	r = inet_ntop(AF_INET6, ones, buf, 38);
	pc_check(r == NULL && errno == ENOSPC, "size 38 for the all-ones address gave %s, errno %s", r ? "a pointer" : "NULL", pc_errname(errno));
	pc_check(inet_ntop(AF_INET6, ones, buf, sizeof buf) != NULL, "a buffer of INET6_ADDRSTRLEN bytes was refused");
	pc_pass();
}
