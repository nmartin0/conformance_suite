/*
 * read() on an unconnected socket gives ENOTCONN.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.read.021
 * MUTANT    read.enotconn_as_eof
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

int main(void)
{
	int s;
	char c;

	pc_begin("XSH.read.021");
	s = socket(AF_UNIX, SOCK_STREAM, 0);
	pc_check(s >= 0, "socket failed");
	alarm(10);
	pc_test();
	pc_expect_err(read(s, &c, 1), ENOTCONN, "read() on an unconnected stream socket");
	pc_pass();
}
