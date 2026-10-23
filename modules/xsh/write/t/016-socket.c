/*
 * write() on a socket is send() without flags.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.016
 * MUTANT    write.socket_drops_last
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

	pc_begin("XSH.write.016");
	pc_check(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0, "socketpair failed");
	alarm(10);
	pc_test();
	pc_check(write(sv[0], "abc", 3) == 3, "write() on the socket failed");
	pc_check(recv(sv[1], buf, sizeof buf, 0) == 3 && memcmp(buf, "abc", 3) == 0,
	    "the peer did not receive the data");
	pc_pass();
}
