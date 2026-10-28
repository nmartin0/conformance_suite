/*
 * When the peer aborts a TCP connection, ECONNRESET becomes the pending
 * error of the socket.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.10-sockets.007
 *
 * On a loopback TCP connection the accepted side sets SO_LINGER to on with
 * a time of 0 and closes, which aborts a connection where the protocol
 * supports it. If the other side then sees an orderly end (recv() returns
 * 0) the connection was not aborted and the test is not in use. Otherwise
 * SO_ERROR on the client reads ECONNRESET, and 0 on a second read.
 */
#include "pcsk.h"

int main(void)
{
	int p[2], r;
	struct linger lg;
	socklen_t l;
	char b[8];

	pc_begin("XSH.2.10-sockets.007");
	guard(60);
	(void)signal(SIGPIPE, SIG_IGN);
	tcp_pair(p);
	lg.l_onoff = 1;
	lg.l_linger = 0;
	NEED(setsockopt(p[1], SOL_SOCKET, SO_LINGER, &lg, sizeof lg) == 0, "setsockopt(SO_LINGER) failed: %s", pc_errname(errno));
	pc_test();
	NEED(close(p[1]) == 0, "close failed: %s", pc_errname(errno));
	NEED(wait_fd(p[0], POLLIN, 5000) > 0, "the client saw nothing of the close within 5 s");
	r = -1;
	l = sizeof r;
	NEED(getsockopt(p[0], SOL_SOCKET, SO_ERROR, &r, &l) == 0, "getsockopt(SO_ERROR) failed: %s", pc_errname(errno));
	if (r == 0) {
		int n = (int)recv(p[0], b, sizeof b, MSG_PEEK);

		if (n == 0)
			pc_notinuse("the close with a linger time of 0 ended the connection in an orderly way");
		pc_check(n != -1 || errno != ECONNRESET, "recv() reported ECONNRESET but SO_ERROR did not");
		pc_fail("neither SO_ERROR nor recv() reported an abort or an orderly end (recv() gave %d)", n);
	}
	pc_check(r == ECONNRESET, "SO_ERROR after the peer aborted the connection gave %s, expected ECONNRESET", pc_errname(r));
	r = -1;
	l = sizeof r;
	pc_check(getsockopt(p[0], SOL_SOCKET, SO_ERROR, &r, &l) == 0, "the second getsockopt(SO_ERROR) failed: %s", pc_errname(errno));
	pc_check(r == 0, "SO_ERROR still gave %s on the second read", r == 0 ? "0" : pc_errname(r));
	pc_pass();
}
