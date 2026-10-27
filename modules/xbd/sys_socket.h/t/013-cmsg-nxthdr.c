/*
 * CMSG_NXTHDR steps to the next header and gives a null pointer after
 *   the last.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_socket.h.013
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
	struct cmsghdr *c1, *c2;
	size_t space = CMSG_SPACE(sizeof(int));

	pc_begin("XBD.sys_socket.h.013");
	pc_test();
	memset(&u, 0, sizeof u);
	memset(&m, 0, sizeof m);
	m.msg_control = u.buf;
	m.msg_controllen = 2 * space;
	c1 = (struct cmsghdr *)u.buf;
	c2 = (struct cmsghdr *)(u.buf + space);
	c1->cmsg_len = CMSG_LEN(sizeof(int));
	c1->cmsg_level = SOL_SOCKET;
	c1->cmsg_type = SCM_RIGHTS;
	c2->cmsg_len = CMSG_LEN(sizeof(int));
	c2->cmsg_level = SOL_SOCKET;
	c2->cmsg_type = SCM_RIGHTS;
	pc_check(CMSG_FIRSTHDR(&m) == c1, "CMSG_FIRSTHDR is not the first header");
	pc_check(IS_T(CMSG_NXTHDR(&m, c1), struct cmsghdr *), "CMSG_NXTHDR does not have type struct cmsghdr *");
	pc_check(CMSG_NXTHDR(&m, c1) == c2, "CMSG_NXTHDR does not step from the first header to the second");
	pc_check(CMSG_NXTHDR(&m, c2) == NULL, "CMSG_NXTHDR after the last header is not null");
	m.msg_controllen = space;
	pc_check(CMSG_NXTHDR(&m, c1) == NULL, "CMSG_NXTHDR after the only header is not null");
	pc_pass();
}
