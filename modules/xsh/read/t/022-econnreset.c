/*
 * read() after a reset gives ECONNRESET.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.read.022
 * MUTANT    read.econnreset_as_eio
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

int main(void)
{
	struct sockaddr_in a;
	socklen_t al = sizeof a;
	struct linger lg;
	int l, c, s;
	char ch;

	pc_begin("XSH.read.022");
	memset(&a, 0, sizeof a);
	a.sin_family = AF_INET;
	a.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	l = socket(AF_INET, SOCK_STREAM, 0);
	if (l < 0 || bind(l, (struct sockaddr *)&a, sizeof a) != 0 || listen(l, 1) != 0 ||
	    getsockname(l, (struct sockaddr *)&a, &al) != 0)
		pc_unresolved("no TCP over loopback");
	c = socket(AF_INET, SOCK_STREAM, 0);
	pc_check(c >= 0 && connect(c, (struct sockaddr *)&a, sizeof a) == 0, "connect failed");
	s = accept(l, NULL, NULL);
	pc_check(s >= 0, "accept failed");
	lg.l_onoff = 1;
	lg.l_linger = 0;
	pc_check(setsockopt(s, SOL_SOCKET, SO_LINGER, &lg, sizeof lg) == 0, "SO_LINGER failed");
	pc_check(close(s) == 0, "close failed");	/* sends a reset */
	{
		struct timespec ts = { 0, 100000000 };
		(void)nanosleep(&ts, NULL);	/* let the reset arrive */
	}
	alarm(10);
	pc_test();
	{
		ssize_t r = read(c, &ch, 1);
		int e = errno;

		if (r == 0)
			pc_unresolved("the zero-linger close did not reset the connection (that is protocol-specific)");
		pc_check(r == -1, "read() after the peer reset the connection returned %ld", (long)r);
		pc_check(e == ECONNRESET || e == ENOTCONN, "read() after a reset failed with %s, expected ECONNRESET (or ENOTCONN)",
		    pc_errname(e));
	}
	pc_pass();
}
