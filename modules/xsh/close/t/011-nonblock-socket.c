/*
 * close() of a full non-blocking socket never reports EAGAIN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.close.011
 * MUTANT    close.eagain_nonblock
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

int main(void)
{
	int sv[2], fl, r, e, i;
	char buf[4096] = { 0 };

	pc_begin("XSH.close.011");
	pc_check(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0, "socketpair failed");
	fl = fcntl(sv[0], F_GETFL);
	pc_check(fl != -1 && fcntl(sv[0], F_SETFL, fl | O_NONBLOCK) == 0,
	    "cannot set O_NONBLOCK");
	for (i = 0; i < 100000 && write(sv[0], buf, sizeof buf) > 0; i++)
		;
	pc_check(errno == EAGAIN || errno == EWOULDBLOCK,
	    "could not fill the send buffer");
	pc_test();
	r = close(sv[0]);
	e = errno;
	pc_check(!(r == -1 && (e == EAGAIN || e == EWOULDBLOCK)),
	    "close() failed with %s", pc_errname(e));
	pc_expect_err(fcntl(sv[0], F_GETFD), EBADF, "fcntl(F_GETFD) after close()");
	pc_pass();
}
