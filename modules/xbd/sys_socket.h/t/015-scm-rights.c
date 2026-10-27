/*
 * SCM_RIGHTS and SOL_SOCKET can be stored as cmsg_type and cmsg_level.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_socket.h.015
 */
#include <sys/socket.h>
#include "pc.h"


int main(void)
{
	struct cmsghdr c;

	pc_begin("XBD.sys_socket.h.015");
	pc_test();
#if !defined(SCM_RIGHTS) || !defined(SOL_SOCKET)
	pc_fail("<sys/socket.h> does not define SCM_RIGHTS and SOL_SOCKET");
#else
	c.cmsg_level = SOL_SOCKET;
	c.cmsg_type = SCM_RIGHTS;
	pc_check(c.cmsg_level == SOL_SOCKET && c.cmsg_type == SCM_RIGHTS, "SOL_SOCKET or SCM_RIGHTS changes when stored in a struct cmsghdr");
	pc_pass();
#endif
}
