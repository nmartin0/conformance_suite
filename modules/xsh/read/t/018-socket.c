/*
 * read() on a socket is recv() without flags.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.read.018
 * MUTANT    read.socket_drops_last
 * VARIANT   read.short_stream
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

int main(void)
{
	int sv[2];
	char buf[8];
	size_t got = 0;
	ssize_t r = 0;

	pc_begin("XSH.read.018");
	pc_check(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0, "socketpair failed");
	pc_check(send(sv[1], "abc", 3, 0) == 3, "send failed");
	/* The peer then stops sending, so the reads below end at end of file. */
	pc_check(shutdown(sv[1], SHUT_WR) == 0, "shutdown failed");
	alarm(10);
	pc_test();
	/*
	 * A stream socket may hand the data over in parts (XSH recv(): data
	 * is returned as it becomes available, with no boundaries), so read
	 * until end of file.
	 */
	while (got < sizeof buf && (r = read(sv[0], buf + got, sizeof buf - got)) > 0)
		got += (size_t)r;
	pc_check(r >= 0, "read() on the socket failed with %s", pc_errname(errno));
	pc_check(got == 3 && memcmp(buf, "abc", 3) == 0,
	    "read() on the socket returned %ld bytes in all, expected the 3 sent", (long)got);
	pc_pass();
}
