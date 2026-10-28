/*
 * With SO_LINGER enabled, close() waits for unsent data until the linger
 * time ends, and no longer.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.10-sockets.012
 *
 * On a loopback TCP connection whose peer never reads, the sender's queues
 * are filled until a non-blocking send() would block. With SO_LINGER on
 * and a time of 1 s, close() of the blocking sender takes at least half a
 * second (the data cannot be sent) and at most 10 s (a cap that only catches a hang).
 */
#include "pcsk.h"

int main(void)
{
	int p[2], r;
	struct linger lg;
	char buf[65536];
	long long t0, t1;
	long total = 0;

	pc_begin("XSH.2.10-sockets.012");
	guard(120);
	(void)signal(SIGPIPE, SIG_IGN);
	memset(buf, 'q', sizeof buf);
	tcp_pair(p);
	set_nb(p[0], 1);
	for (;;) {
		r = (int)send(p[0], buf, sizeof buf, MSG_NOSIGNAL);
		if (r < 0)
			break;
		total += r;
		NEED(total < (1L << 30), "the queues did not fill after 1 GiB");
	}
	NEED(again(errno), "send failed: %s", pc_errname(errno));
	nap_ms(300);
	r = (int)send(p[0], buf, sizeof buf, MSG_NOSIGNAL);
	NEED(r < 0 && again(errno), "the send queue is not full");
	set_nb(p[0], 0);
	lg.l_onoff = 1;
	lg.l_linger = 1;
	NEED(setsockopt(p[0], SOL_SOCKET, SO_LINGER, &lg, sizeof lg) == 0, "setsockopt(SO_LINGER) failed: %s", pc_errname(errno));
	pc_test();
	t0 = mono_ms();
	r = close(p[0]);
	t1 = mono_ms();
	(void)r;	/* whether close() reports an error is not what is judged here */
	pc_check(t1 - t0 >= 500, "close() with SO_LINGER on and unsendable data returned after %lld ms, expected to wait about the linger time of 1 s", t1 - t0);
	pc_check(t1 - t0 <= 10000, "close() with a linger time of 1 s took %lld ms", t1 - t0);
	pc_pass();
}
