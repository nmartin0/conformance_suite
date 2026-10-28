/*
 * The owner of a new socket is unset.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.10-sockets.003
 *
 * F_GETOWN of a new stream socket, datagram socket and socketpair() end
 * (AF_UNIX and AF_INET) gives 0, the value that means no process or
 * process group receives SIGURG.
 */
#include "pcsk.h"

int main(void)
{
	int fds[6], sv[2], i, r;
	static const char *what[6] = { "AF_UNIX stream", "AF_UNIX datagram", "AF_INET stream", "AF_INET datagram", "socketpair end 0", "socketpair end 1" };

	pc_begin("XSH.2.10-sockets.003");
	guard(60);
	fds[0] = socket(AF_UNIX, SOCK_STREAM, 0);
	fds[1] = socket(AF_UNIX, SOCK_DGRAM, 0);
	fds[2] = socket(AF_INET, SOCK_STREAM, 0);
	fds[3] = socket(AF_INET, SOCK_DGRAM, 0);
	NEED(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0, "socketpair failed: %s", pc_errname(errno));
	fds[4] = sv[0];
	fds[5] = sv[1];
	for (i = 0; i < 6; i++)
		NEED(fds[i] != -1, "socket failed: %s", pc_errname(errno));
	pc_test();
	for (i = 0; i < 6; i++) {
		r = fcntl(fds[i], F_GETOWN);
		pc_check(r != -1, "F_GETOWN failed on a %s socket: %s", what[i], pc_errname(errno));
		pc_check(r == 0, "F_GETOWN of a new %s socket gave %d, expected 0 (no owner)", what[i], r);
	}
	pc_pass();
}
