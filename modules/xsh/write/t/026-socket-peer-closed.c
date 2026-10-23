/*
 * Writing to a stream socket whose peer is gone gives EPIPE and SIGPIPE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.026
 * MUTANT    write.epipe_as_eio
 * MUTANT    write.sigpipe_suppressed
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

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

	pc_begin("XSH.write.026");
	pc_check(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0, "socketpair failed");
	pc_check(close(sv[1]) == 0, "close of the peer failed");
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_pipe;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGPIPE, &sa, NULL) == 0, "sigaction failed");
	alarm(10);
	pc_test();
	pc_expect_err(write(sv[0], "x", 1), EPIPE, "write() on a stream socket whose peer is gone");
	pc_check(got_pipe, "SIGPIPE was not sent");
	pc_pass();
}
