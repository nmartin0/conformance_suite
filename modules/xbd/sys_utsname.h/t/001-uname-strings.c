/*
 * The utsname members are null-terminated after uname().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_utsname.h.001
 */
#include <sys/utsname.h>
#include <string.h>
#include "pc.h"

#define TERMINATED(m) (memchr(u.m, '\0', sizeof u.m) != NULL)

int main(void)
{
	pc_begin("XBD.sys_utsname.h.001");
	pc_test();
	struct utsname u;
	int r;

	memset(&u, 'x', sizeof u);
	r = uname(&u);
	pc_check(r != -1, "uname() failed");
	pc_check(TERMINATED(sysname), "sysname is not null-terminated inside its array");
	pc_check(TERMINATED(nodename), "nodename is not null-terminated inside its array");
	pc_check(TERMINATED(release), "release is not null-terminated inside its array");
	pc_check(TERMINATED(version), "version is not null-terminated inside its array");
	pc_check(TERMINATED(machine), "machine is not null-terminated inside its array");
	pc_pass();
}
