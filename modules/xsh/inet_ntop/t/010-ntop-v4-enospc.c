/*
 * inet_ntop() with a too small IPv4 buffer returns a null pointer and sets ENOSPC.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.inet_ntop.010
 */
#include "ip.h"

int main(void)
{
	static const unsigned char a[4] = { 255, 255, 255, 255 };
	static const socklen_t sizes[] = { 0, 1, 7, 15 };
	char buf[INET_ADDRSTRLEN];
	size_t i;

	pc_begin("XSH.inet_ntop.010");
	pc_test();
	for (i = 0; i < 4; i++) {
		const char *r;

		errno = 0;
		r = inet_ntop(AF_INET, a, buf, sizes[i]);
		pc_check(r == NULL && errno == ENOSPC, "size %u gave %s, errno %s; expected NULL and ENOSPC", (unsigned)sizes[i], r ? "a pointer" : "NULL", pc_errname(errno));
	}
	pc_check(inet_ntop(AF_INET, a, buf, sizeof buf) != NULL, "a buffer of INET_ADDRSTRLEN bytes was refused");
	pc_pass();
}
