/*
 * read() on a socket is recv() without flags.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.read.018
 * MUTANT    read.socket_drops_last
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
	ssize_t r;

	pc_begin("XSH.read.018");
	pc_check(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0, "socketpair failed");
	pc_check(send(sv[1], "abc", 3, 0) == 3, "send failed");
	alarm(10);
	pc_test();
	r = read(sv[0], buf, sizeof buf);
	pc_check(r == 3 && memcmp(buf, "abc", 3) == 0, "read() on the socket returned %ld", (long)r);
	pc_pass();
}
