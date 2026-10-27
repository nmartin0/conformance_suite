/*
 * CMSG_NXTHDR with a null second argument is CMSG_FIRSTHDR.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_socket.h.016
 */
#include <sys/socket.h>
#include <string.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

/* Room for two small objects, aligned for a struct cmsghdr. */
static union {
	struct cmsghdr align;
	unsigned char buf[2 * CMSG_SPACE(sizeof(int)) + 64];
} u;

int main(void)
{
	struct msghdr m;
	struct cmsghdr *none = NULL;

	pc_begin("XBD.sys_socket.h.016");
	pc_test();
	memset(&u, 0, sizeof u);
	memset(&m, 0, sizeof m);
	m.msg_control = u.buf;
	m.msg_controllen = CMSG_SPACE(sizeof(int));
	pc_check(CMSG_NXTHDR(&m, none) == CMSG_FIRSTHDR(&m), "CMSG_NXTHDR(mhdr, NULL) differs from CMSG_FIRSTHDR(mhdr)");
	pc_check(CMSG_NXTHDR(&m, none) == (struct cmsghdr *)u.buf, "CMSG_NXTHDR(mhdr, NULL) is not the start of the control data");
	m.msg_controllen = 0;
	pc_check(CMSG_NXTHDR(&m, none) == NULL, "CMSG_NXTHDR(mhdr, NULL) is not null for no control data");
	pc_pass();
}
