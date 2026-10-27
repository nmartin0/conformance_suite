/*
 * uname() fills the utsname structure.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.uname.001
 */
#include "pc.h"
#include <string.h>
#include <sys/utsname.h>

int main(void)
{
	struct utsname u;
	int r;

	pc_begin("XSH.uname.001");
	memset(&u, 0xFF, sizeof u);
	pc_test();
	r = uname(&u);
	pc_check(r >= 0, "uname() returned %d", r);
	pc_check(memchr(u.sysname, '\0', sizeof u.sysname) != NULL, "sysname is not null-terminated");
	pc_check(memchr(u.nodename, '\0', sizeof u.nodename) != NULL, "nodename is not null-terminated");
	pc_check(memchr(u.release, '\0', sizeof u.release) != NULL, "release is not null-terminated");
	pc_check(memchr(u.version, '\0', sizeof u.version) != NULL, "version is not null-terminated");
	pc_check(memchr(u.machine, '\0', sizeof u.machine) != NULL, "machine is not null-terminated");
	pc_check(u.sysname[0] != '\0', "sysname is empty");
	pc_check(u.release[0] != '\0', "release is empty");
	pc_check(u.version[0] != '\0', "version is empty");
	pc_check(u.machine[0] != '\0', "machine is empty");
	pc_pass();
}
