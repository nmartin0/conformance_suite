/*
 * The default values of the socket options on datagram sockets.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.10-sockets.013
 *
 * On a new UDP socket and a new AF_UNIX datagram socket: SO_BROADCAST, SO_DEBUG
 * and SO_REUSEADDR are 0 (off), SO_RCVTIMEO and SO_SNDTIMEO are zero. An
 * option the socket does not support (ENOPROTOOPT) is left out; at least
 * SO_BROADCAST on the UDP socket and one more must be checked.
 */
#include "pcsk.h"

static int checked;

static void int_zero(int s, int opt, const char *name, const char *what)
{
	int v = -7;
	socklen_t l = sizeof v;

	if (getsockopt(s, SOL_SOCKET, opt, &v, &l) == -1) {
		pc_check(errno == ENOPROTOOPT || errno == EINVAL || errno == EOPNOTSUPP, "getsockopt(%s) on %s failed: %s", name, what, pc_errname(errno));
		return;
	}
	pc_check(v == 0, "the default of %s on %s is %d, expected 0", name, what, v);
	checked++;
}

static void time_zero(int s, int opt, const char *name, const char *what)
{
	struct timeval tv;
	socklen_t l = sizeof tv;

	tv.tv_sec = 9;
	tv.tv_usec = 9;
	if (getsockopt(s, SOL_SOCKET, opt, &tv, &l) == -1) {
		pc_check(errno == ENOPROTOOPT || errno == EINVAL || errno == EOPNOTSUPP, "getsockopt(%s) on %s failed: %s", name, what, pc_errname(errno));
		return;
	}
	pc_check(tv.tv_sec == 0 && tv.tv_usec == 0, "the default of %s on %s is not zero", name, what);
	checked++;
}

int main(void)
{
	int u, d, k;
	int bc;
	socklen_t l;

	pc_begin("XSH.2.10-sockets.013");
	guard(60);
	u = socket(AF_INET, SOCK_DGRAM, 0);
	d = socket(AF_UNIX, SOCK_DGRAM, 0);
	NEED(u != -1 && d != -1, "socket failed: %s", pc_errname(errno));
	pc_test();
	bc = -7;
	l = sizeof bc;
	k = getsockopt(u, SOL_SOCKET, SO_BROADCAST, &bc, &l);
	pc_check(k == 0, "getsockopt(SO_BROADCAST) on a UDP socket failed: %s", pc_errname(errno));
	pc_check(bc == 0, "the default of SO_BROADCAST on a UDP socket is %d, expected 0 (disabled)", bc);
	checked = 1;
	int_zero(u, SO_DEBUG, "SO_DEBUG", "a UDP socket");
	int_zero(u, SO_REUSEADDR, "SO_REUSEADDR", "a UDP socket");
	time_zero(u, SO_RCVTIMEO, "SO_RCVTIMEO", "a UDP socket");
	time_zero(u, SO_SNDTIMEO, "SO_SNDTIMEO", "a UDP socket");
	int_zero(d, SO_DEBUG, "SO_DEBUG", "an AF_UNIX datagram socket");
	int_zero(d, SO_REUSEADDR, "SO_REUSEADDR", "an AF_UNIX datagram socket");
	time_zero(d, SO_RCVTIMEO, "SO_RCVTIMEO", "an AF_UNIX datagram socket");
	time_zero(d, SO_SNDTIMEO, "SO_SNDTIMEO", "an AF_UNIX datagram socket");
	pc_check(checked >= 2, "no option besides SO_BROADCAST could be read");
	pc_pass();
}
