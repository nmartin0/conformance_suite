/*
 * write() on a socket is send() without flags.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.016
 * MUTANT    write.socket_drops_last
 * VARIANT   read.short_stream
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

int main(void)
{
	int sv[2];
	char buf[8];
	size_t got = 0;
	ssize_t r = 0;

	pc_begin("XSH.write.016");
	pc_check(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0, "socketpair failed");
	alarm(10);
	pc_test();
	pc_check(write(sv[0], "abc", 3) == 3, "write() on the socket failed");
	/*
	 * The peer may receive a stream in parts (XSH recv(): data is returned
	 * as it becomes available), so it reads until end of file, which the
	 * writer's shutdown() marks after the data.
	 */
	pc_check(shutdown(sv[0], SHUT_WR) == 0, "shutdown failed");
	while (got < sizeof buf && (r = recv(sv[1], buf + got, sizeof buf - got, 0)) > 0)
		got += (size_t)r;
	pc_check(r >= 0 && got == 3 && memcmp(buf, "abc", 3) == 0,
	    "the peer received %ld bytes (%s), expected the 3 written", (long)got,
	    r < 0 ? pc_errname(errno) : "-");
	pc_pass();
}
