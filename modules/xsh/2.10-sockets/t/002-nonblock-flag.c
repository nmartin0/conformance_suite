/*
 * O_NONBLOCK is off on a new socket, belongs to the open file description,
 * and is set and cleared with F_SETFL.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.10-sockets.002
 *
 * A socket from socket() and one end of a socketpair() are checked: the
 * flag is clear, F_SETFL sets it, a dup() of the descriptor sees it (same
 * description) and clears it again, and the other end of the pair, a
 * different description, is not affected.
 */
#include "pcsk.h"

static int nb(int fd)
{
	int fl = fcntl(fd, F_GETFL);

	NEED(fl != -1, "F_GETFL failed: %s", pc_errname(errno));
	return (fl & O_NONBLOCK) != 0;
}

int main(void)
{
	int s, d, sv[2], fl;
	char b;

	pc_begin("XSH.2.10-sockets.002");
	guard(60);
	s = socket(AF_UNIX, SOCK_STREAM, 0);
	NEED(s != -1, "socket failed: %s", pc_errname(errno));
	NEED(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0, "socketpair failed: %s", pc_errname(errno));
	d = dup(s);
	NEED(d != -1, "dup failed: %s", pc_errname(errno));
	pc_test();

	pc_check(nb(s) == 0, "O_NONBLOCK is set on a new socket");
	fl = fcntl(s, F_GETFL);
	pc_check(fcntl(s, F_SETFL, fl | O_NONBLOCK) == 0, "F_SETFL with O_NONBLOCK failed: %s", pc_errname(errno));
	pc_check(nb(s) == 1, "O_NONBLOCK is not set after F_SETFL");
	pc_check(nb(d) == 1, "a dup() of the descriptor does not see O_NONBLOCK, which belongs to the open file description");
	fl = fcntl(d, F_GETFL);
	pc_check(fcntl(d, F_SETFL, fl & ~O_NONBLOCK) == 0, "F_SETFL clearing O_NONBLOCK failed: %s", pc_errname(errno));
	pc_check(nb(s) == 0 && nb(d) == 0, "O_NONBLOCK is still set after F_SETFL cleared it");

	pc_check(nb(sv[0]) == 0 && nb(sv[1]) == 0, "O_NONBLOCK is set on a new socketpair() end");
	fl = fcntl(sv[0], F_GETFL);
	pc_check(fcntl(sv[0], F_SETFL, fl | O_NONBLOCK) == 0, "F_SETFL failed on a socketpair() end: %s", pc_errname(errno));
	pc_check(nb(sv[0]) == 1, "O_NONBLOCK is not set on the end it was set on");
	pc_check(nb(sv[1]) == 0, "O_NONBLOCK of the other end of the pair changed");
	pc_check(recv(sv[0], &b, 1, 0) == -1 && again(errno), "recv() on the empty end with O_NONBLOCK set did not fail with EAGAIN or EWOULDBLOCK: %s", en(-1, errno));
	pc_pass();
}
