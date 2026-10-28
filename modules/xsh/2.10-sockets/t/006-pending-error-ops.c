/*
 * A pending socket error is returned by a send() or a recv() and is then
 * cleared.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.10-sockets.006
 *
 * A non-blocking TCP connect() to a loopback port nobody listens on ends
 * with ECONNREFUSED as the pending error. Twice (once with recv(), once
 * with send()) a fresh socket in that state is used: the call fails with
 * ECONNREFUSED, and SO_ERROR then reads 0. A call that fails with ENOTCONN
 * or EPIPE instead is giving another error that applies to a socket that
 * is not connected (XSH 2.3 leaves the choice open), and leaves the test
 * not in use, as does a connect() that fails at once.
 */
#include "pcsk.h"

int main(void)
{
	struct sockaddr_in a;
	int way, c, r, e;
	socklen_t l;
	char b;

	pc_begin("XSH.2.10-sockets.006");
	guard(60);
	(void)signal(SIGPIPE, SIG_IGN);
	pc_test();
	for (way = 0; way < 2; way++) {
		closed_port(&a);
		c = socket(AF_INET, SOCK_STREAM, 0);
		NEED(c != -1, "socket failed: %s", pc_errname(errno));
		set_nb(c, 1);
		r = connect(c, (struct sockaddr *)&a, sizeof a);
		NEED(r == -1, "connect() to a closed port succeeded");
		if (errno == ECONNREFUSED)
			pc_notinuse("connect() failed at once, so no error is pending afterwards");
		NEED(errno == EINPROGRESS, "connect() failed with %s", pc_errname(errno));
		NEED(wait_fd(c, POLLOUT, 5000) > 0, "the connection attempt did not complete within 5 s");
		if (way == 0)
			r = (int)recv(c, &b, 1, 0);
		else
			r = (int)send(c, "x", 1, MSG_NOSIGNAL);
		e = errno;
		if (r == -1 && (e == ENOTCONN || e == EPIPE))
			pc_notinuse("the call reported %s (also applicable to a socket that is not connected) rather than the pending error", pc_errname(e));
		pc_check(r == -1, "%s on a socket whose connection was refused succeeded", way == 0 ? "recv()" : "send()");
		pc_check(e == ECONNREFUSED, "%s on a socket whose connection was refused failed with %s, expected the pending error ECONNREFUSED",
		    way == 0 ? "recv()" : "send()", pc_errname(e));
		r = -1;
		l = sizeof r;
		pc_check(getsockopt(c, SOL_SOCKET, SO_ERROR, &r, &l) == 0, "getsockopt(SO_ERROR) failed: %s", pc_errname(errno));
		pc_check(r == 0, "SO_ERROR still gave %s after %s had returned the pending error", r == 0 ? "0" : pc_errname(r),
		    way == 0 ? "recv()" : "send()");
		(void)close(c);
	}
	pc_pass();
}
