/*
 * write() on an unconnected stream socket gives ENOTCONN or ECONNRESET.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.027
 * MUTANT    write.notconn_ok
 * VARIANT   write.unconn_econnreset
 */
#include "pc.h"
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>

int main(void)
{
	int s, e;
	ssize_t r;

	pc_begin("XSH.write.027");
	s = socket(AF_UNIX, SOCK_STREAM, 0);
	pc_check(s >= 0, "socket failed");
	signal(SIGPIPE, SIG_IGN);
	alarm(10);
	pc_test();
	r = write(s, "x", 1);
	e = errno;
	pc_check(r == -1 && (e == ENOTCONN || e == ECONNRESET),
	    "write() on an unconnected stream socket gave %ld (%s), expected ENOTCONN or ECONNRESET",
	    (long)r, r < 0 ? pc_errname(e) : "-");
	pc_pass();
}
