/*
 * SIGURG is sent to the owner of a socket when out-of-band data arrives,
 * and to nobody when the socket has no owner.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.10-sockets.009
 *
 * Two loopback TCP connections. On the first, the receiving socket has no
 * owner; the peer sends a byte with MSG_OOB and no SIGURG may be caught in
 * the next second. On the second, the receiving socket's owner is this
 * process (F_SETOWN); after the peer's MSG_OOB byte SIGURG must be caught
 * within 5 s. Out-of-band data is protocol-specific: where sending it is
 * refused the test is not in use.
 */
#include "pcsk.h"

static volatile sig_atomic_t urg;

static void on_urg(int sig)
{
	(void)sig;
	urg++;
}

static void send_oob(int fd)
{
	if (send(fd, "X", 1, MSG_OOB | MSG_NOSIGNAL) != 1) {
		if (errno == EOPNOTSUPP || errno == EINVAL || errno == ENOPROTOOPT || errno == EAFNOSUPPORT)
			pc_notinuse("sending out-of-band data is not supported on a loopback TCP connection (%s)", pc_errname(errno));
		pc_unresolved("send with MSG_OOB failed: %s", pc_errname(errno));
	}
}

int main(void)
{
	struct sigaction sa;
	int a[2], b[2], i;

	pc_begin("XSH.2.10-sockets.009");
	guard(60);
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_urg;
	sigemptyset(&sa.sa_mask);
	NEED(sigaction(SIGURG, &sa, NULL) == 0, "sigaction failed: %s", pc_errname(errno));
	tcp_pair(a);
	tcp_pair(b);
	NEED(fcntl(a[1], F_GETOWN) == 0, "a new socket has an owner");
	NEED(fcntl(b[1], F_SETOWN, (int)getpid()) == 0, "F_SETOWN failed: %s", pc_errname(errno));
	pc_test();

	send_oob(a[0]);
	for (i = 0; i < 10; i++)
		nap_ms(100);
	pc_check(urg == 0, "SIGURG was sent for a socket that has no owner");

	send_oob(b[0]);
	for (i = 0; i < 50 && urg == 0; i++)
		nap_ms(100);
	pc_check(urg > 0, "SIGURG did not reach the owner of the socket within 5 s of out-of-band data arriving");
	pc_pass();
}
