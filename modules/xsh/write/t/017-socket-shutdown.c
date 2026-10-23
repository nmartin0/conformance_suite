/*
 * Writing to a socket shut down for writing gives EPIPE and SIGPIPE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.017
 * MUTANT    write.epipe_as_eio
 * MUTANT    write.sigpipe_suppressed
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>

static volatile sig_atomic_t got_pipe;

static void on_pipe(int sig)
{
	(void)sig;
	got_pipe = 1;
}

int main(void)
{
	struct sigaction sa;
	int sv[2];

	pc_begin("XSH.write.017");
	pc_check(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0, "socketpair failed");
	pc_check(shutdown(sv[0], SHUT_WR) == 0, "shutdown failed");
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_pipe;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGPIPE, &sa, NULL) == 0, "sigaction failed");
	pc_test();
	pc_expect_err(write(sv[0], "x", 1), EPIPE, "write() on a socket shut down for writing");
	pc_check(got_pipe, "SIGPIPE was not sent");
	pc_pass();
}
