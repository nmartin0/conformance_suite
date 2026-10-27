/*
 * CMSG_FIRSTHDR gives a null pointer for no or too little room, and the
 *   start of the control data otherwise.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_socket.h.012
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

	pc_begin("XBD.sys_socket.h.012");
	pc_test();
	memset(&m, 0, sizeof m);
	m.msg_control = u.buf;
	m.msg_controllen = 0;
	pc_check(CMSG_FIRSTHDR(&m) == NULL, "CMSG_FIRSTHDR is not null for msg_controllen 0");
	m.msg_control = NULL;
	pc_check(CMSG_FIRSTHDR(&m) == NULL, "CMSG_FIRSTHDR is not null for no control data");
	m.msg_control = u.buf;
	m.msg_controllen = sizeof(struct cmsghdr) - 1;
	pc_check(CMSG_FIRSTHDR(&m) == NULL, "CMSG_FIRSTHDR is not null for msg_controllen %lu, less than sizeof(struct cmsghdr)", (unsigned long)m.msg_controllen);
	pc_check(IS_T(CMSG_FIRSTHDR(&m), struct cmsghdr *), "CMSG_FIRSTHDR does not have type struct cmsghdr *");
	m.msg_controllen = sizeof(struct cmsghdr);
	pc_check(CMSG_FIRSTHDR(&m) == (struct cmsghdr *)m.msg_control, "CMSG_FIRSTHDR is not msg_control for msg_controllen sizeof(struct cmsghdr)");
	m.msg_controllen = CMSG_SPACE(sizeof(int));
	pc_check(CMSG_FIRSTHDR(&m) == (struct cmsghdr *)m.msg_control, "CMSG_FIRSTHDR is not msg_control for msg_controllen CMSG_SPACE(sizeof(int))");
	pc_pass();
}
