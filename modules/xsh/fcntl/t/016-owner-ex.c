/*
 * F_SETOWN_EX and F_GETOWN_EX on a socket.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fcntl.016
 * MUTANT    fcntl.setown_ex_ignored
 * MUTANT    fcntl.getown_ex_type_swapped
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

int main(void)
{
	int sv[2];
	struct f_owner_ex o;

	pc_begin("XSH.fcntl.016");
	pc_check(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0, "socketpair failed");
	pc_test();
	o.type = F_OWNER_PID;
	o.pid = getpid();
	pc_expect_ok(fcntl(sv[0], F_SETOWN_EX, &o), "F_SETOWN_EX (F_OWNER_PID)");
	o.type = -1;
	o.pid = -1;
	pc_expect_ok(fcntl(sv[0], F_GETOWN_EX, &o), "F_GETOWN_EX");
	pc_check(o.type == F_OWNER_PID && o.pid == getpid(),
	    "F_GETOWN_EX gave type %d pid %ld", (int)o.type, (long)o.pid);
	o.type = F_OWNER_PGRP;
	o.pid = getpgrp();
	pc_expect_ok(fcntl(sv[0], F_SETOWN_EX, &o), "F_SETOWN_EX (F_OWNER_PGRP)");
	o.type = -1;
	o.pid = -1;
	pc_expect_ok(fcntl(sv[0], F_GETOWN_EX, &o), "F_GETOWN_EX");
	pc_check(o.type == F_OWNER_PGRP && o.pid == getpgrp(),
	    "F_GETOWN_EX gave type %d pid %ld", (int)o.type, (long)o.pid);
	o.type = F_OWNER_PID;
	o.pid = 0;
	pc_expect_ok(fcntl(sv[0], F_SETOWN_EX, &o), "F_SETOWN_EX (pid 0)");
	o.pid = -1;
	pc_expect_ok(fcntl(sv[0], F_GETOWN_EX, &o), "F_GETOWN_EX");
	pc_check(o.pid == 0, "F_GETOWN_EX gave pid %ld, expected 0", (long)o.pid);
	pc_pass();
}
