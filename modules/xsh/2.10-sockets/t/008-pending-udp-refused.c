/*
 * A datagram refused by the destination becomes the pending error of a
 * connected datagram socket.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.10-sockets.008
 *
 * A UDP socket connected to a loopback port nobody uses sends a datagram.
 * Whether the system detects and reports the refusal is up to the network
 * layer; if no error shows up within 2 s the test is not in use. If one
 * does, SO_ERROR gives ECONNREFUSED, and 0 when read again.
 */
#include "pcsk.h"

int main(void)
{
	struct sockaddr_in a, dst;
	socklen_t l = sizeof a;
	int dead, u, r, i;

	pc_begin("XSH.2.10-sockets.008");
	guard(60);
	(void)signal(SIGPIPE, SIG_IGN);
	dead = socket(AF_INET, SOCK_DGRAM, 0);
	u = socket(AF_INET, SOCK_DGRAM, 0);
	NEED(dead != -1 && u != -1, "socket failed: %s", pc_errname(errno));
	memset(&a, 0, sizeof a);
	a.sin_family = AF_INET;
	a.sin_addr.s_addr = pc_loopback4();
	NEED(bind(dead, (struct sockaddr *)&a, sizeof a) == 0, "bind failed: %s", pc_errname(errno));
	NEED(getsockname(dead, (struct sockaddr *)&dst, &l) == 0, "getsockname failed: %s", pc_errname(errno));
	(void)close(dead);
	NEED(connect(u, (struct sockaddr *)&dst, sizeof dst) == 0, "connect of the datagram socket failed: %s", pc_errname(errno));
	pc_test();
	NEED(send(u, "x", 1, 0) == 1, "send failed: %s", pc_errname(errno));
	r = 0;
	for (i = 0; i < 20 && r == 0; i++) {
		socklen_t rl = sizeof r;

		nap_ms(100);
		pc_check(getsockopt(u, SOL_SOCKET, SO_ERROR, &r, &rl) == 0, "getsockopt(SO_ERROR) failed: %s", pc_errname(errno));
	}
	if (r == 0)
		pc_notinuse("no error was reported for the refused datagram within 2 s");
	pc_check(r == ECONNREFUSED, "the pending error after a datagram to a closed port was %s, expected ECONNREFUSED", pc_errname(r));
	r = -1;
	l = sizeof r;
	pc_check(getsockopt(u, SOL_SOCKET, SO_ERROR, &r, &l) == 0, "the second getsockopt(SO_ERROR) failed: %s", pc_errname(errno));
	pc_check(r == 0, "SO_ERROR still gave %s on the second read", r == 0 ? "0" : pc_errname(r));
	pc_pass();
}
