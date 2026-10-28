/*
 * With SO_OOBINLINE set, out-of-band data is returned by an ordinary
 * receive, in order with the other data.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.10-sockets.011
 *
 * On a loopback TCP connection the receiving socket has SO_OOBINLINE set;
 * the peer sends "abc", one byte 'X' with MSG_OOB, then "def". Reading
 * with recv() without MSG_OOB (a mark, if the protocol keeps one, ends a
 * receive but is not returned) yields "abcXdef". Where out-of-band data
 * is refused the test is not in use.
 */
#include "pcsk.h"

int main(void)
{
	int p[2], on = 1, got = 0, i;
	char buf[32];
	ssize_t n;

	pc_begin("XSH.2.10-sockets.011");
	guard(60);
	(void)signal(SIGPIPE, SIG_IGN);
	(void)signal(SIGURG, SIG_IGN);
	tcp_pair(p);
	NEED(setsockopt(p[1], SOL_SOCKET, SO_OOBINLINE, &on, sizeof on) == 0, "setsockopt(SO_OOBINLINE) failed: %s", pc_errname(errno));
	NEED(send(p[0], "abc", 3, MSG_NOSIGNAL) == 3, "send failed: %s", pc_errname(errno));
	if (send(p[0], "X", 1, MSG_OOB | MSG_NOSIGNAL) != 1) {
		if (errno == EOPNOTSUPP || errno == EINVAL || errno == ENOPROTOOPT)
			pc_notinuse("sending out-of-band data is not supported on a loopback TCP connection (%s)", pc_errname(errno));
		pc_unresolved("send with MSG_OOB failed: %s", pc_errname(errno));
	}
	NEED(send(p[0], "def", 3, MSG_NOSIGNAL) == 3, "send failed: %s", pc_errname(errno));
	pc_test();

	for (i = 0; i < 10 && got < 7; i++) {
		if (wait_fd(p[1], POLLIN, 1000) <= 0)
			continue;
		n = recv(p[1], buf + got, sizeof buf - (size_t)got, 0);
		pc_check(n > 0, "recv() returned %ld", (long)n);
		got += (int)n;
	}
	pc_check(got == 7 && memcmp(buf, "abcXdef", 7) == 0, "with SO_OOBINLINE the data read was %d bytes (\"%.*s\"), expected \"abcXdef\"", got, got, buf);
	pc_pass();
}
