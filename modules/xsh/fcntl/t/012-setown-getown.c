/*
 * F_SETOWN and F_GETOWN on a socket.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.fcntl.012
 * MUTANT    fcntl.setown_ignored
 * MUTANT    fcntl.getown_zero
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

int main(void)
{
	int sv[2], r;
	pid_t me, pg;

	pc_begin("XSH.fcntl.012");
	pc_check(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0, "socketpair failed");
	me = getpid();
	pg = getpgrp();
	pc_check(pg > 1, "process group %ld cannot be told from -1", (long)pg);
	pc_test();
	pc_expect_ok(fcntl(sv[0], F_SETOWN, (int)me), "fcntl(F_SETOWN, getpid())");
	r = fcntl(sv[0], F_GETOWN);
	pc_check(r == (int)me, "F_GETOWN gave %d, expected %ld", r, (long)me);
	pc_expect_ok(fcntl(sv[0], F_SETOWN, -(int)pg), "fcntl(F_SETOWN, -getpgrp())");
	r = fcntl(sv[0], F_GETOWN);
	pc_check(r == -(int)pg, "F_GETOWN gave %d, expected %ld", r, -(long)pg);
	pc_expect_ok(fcntl(sv[0], F_SETOWN, 0), "fcntl(F_SETOWN, 0)");
	r = fcntl(sv[0], F_GETOWN);
	pc_check(r == 0, "F_GETOWN gave %d, expected 0", r);
	pc_pass();
}
