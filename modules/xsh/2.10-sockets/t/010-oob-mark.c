/*
 * An out-of-band mark in the receive queue separates the data before it
 * from the data after it, is never returned as data, and is removed by a
 * receive made when it is first.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.10-sockets.010
 *
 * On a loopback TCP connection the peer sends "abc", one byte with
 * MSG_OOB, then "def". Whether the protocol puts a mark in the queue is
 * protocol-specific: the test is not in use if sending out-of-band data
 * is refused or if no mark is ever seen by sockatmark(). With a mark: a
 * receive for more than three bytes returns at most "abc" (nothing from
 * after the mark), sockatmark() is 1 once "abc" is consumed, the next
 * receive returns "def" and sockatmark() is then 0.
 */
#include "pcsk.h"

int main(void)
{
	int p[2], m, n, got = 0, k;
	char buf[32];

	pc_begin("XSH.2.10-sockets.010");
	guard(60);
	(void)signal(SIGPIPE, SIG_IGN);
	(void)signal(SIGURG, SIG_IGN);
	tcp_pair(p);
	NEED(send(p[0], "abc", 3, MSG_NOSIGNAL) == 3, "send failed: %s", pc_errname(errno));
	if (send(p[0], "X", 1, MSG_OOB | MSG_NOSIGNAL) != 1) {
		if (errno == EOPNOTSUPP || errno == EINVAL || errno == ENOPROTOOPT)
			pc_notinuse("sending out-of-band data is not supported on a loopback TCP connection (%s)", pc_errname(errno));
		pc_unresolved("send with MSG_OOB failed: %s", pc_errname(errno));
	}
	NEED(send(p[0], "def", 3, MSG_NOSIGNAL) == 3, "send failed: %s", pc_errname(errno));
	pc_test();

	NEED(wait_fd(p[1], POLLIN, 5000) > 0, "nothing to receive");
	nap_ms(300);
	m = sockatmark(p[1]);
	if (m == -1)
		pc_notinuse("sockatmark() fails on this connection: %s", pc_errname(errno));
	/* read normal data until the mark is first in the queue */
	while (m == 0 && got < 6) {
		if (wait_fd(p[1], POLLIN, 2000) <= 0)
			break;
		n = (int)recv(p[1], buf + got, sizeof buf - (size_t)got, 0);
		pc_check(n > 0, "recv() returned %d before any mark was seen", n);
		got += n;
		m = sockatmark(p[1]);
		pc_check(m != -1, "sockatmark() failed: %s", pc_errname(errno));
	}
	if (m == 0)
		pc_notinuse("the receive queue holds no out-of-band mark");
	pc_check(got == 3 && memcmp(buf, "abc", 3) == 0, "the data read before the mark was %d bytes, expected exactly \"abc\"", got);
	n = (int)recv(p[1], buf, sizeof buf, 0);
	pc_check(n == 3 && memcmp(buf, "def", 3) == 0, "the receive made with the mark first returned %d bytes, expected \"def\"", n);
	k = sockatmark(p[1]);
	pc_check(k == 0, "sockatmark() gave %d after the receive that should have removed the mark", k);
	pc_pass();
}
