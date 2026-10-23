/*
 * Closing one end of a socket pair gives the other end end of file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.close.008
 * MUTANT    close.noop
 * MUTANT    close.shutdown_each
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>


int main(void)
{
	int sv[2], d, fl;
	ssize_t n;
	char c;

	pc_begin("XSH.close.008");
	pc_check(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0, "socketpair failed");
	d = dup(sv[0]);
	pc_check(d >= 0, "dup failed");
	fl = fcntl(sv[1], F_GETFL);
	pc_check(fl != -1 && fcntl(sv[1], F_SETFL, fl | O_NONBLOCK) == 0,
	    "cannot set O_NONBLOCK");
	pc_test();
	pc_expect_ok(close(sv[0]), "close(first descriptor of one end)");
	n = read(sv[1], &c, 1);
	pc_check(n == -1 && (errno == EAGAIN || errno == EWOULDBLOCK),
	    "socket destroyed while a descriptor was open: read gave %ld (%s)",
	    (long)n, n < 0 ? pc_errname(errno) : "-");
	pc_expect_ok(close(d), "close(last descriptor of that end)");
	n = read(sv[1], &c, 1);
	pc_check(n == 0, "read() at the other end gave %ld (%s), expected 0 (end of file)",
	    (long)n, n < 0 ? pc_errname(errno) : "-");
	pc_pass();
}
