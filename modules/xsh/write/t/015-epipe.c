/*
 * Writing to a readerless pipe gives EPIPE and SIGPIPE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.write.015
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

static volatile sig_atomic_t got_pipe;

static void on_pipe(int sig)
{
	(void)sig;
	got_pipe = 1;
}

int main(void)
{
	struct sigaction sa;
	int p[2];

	pc_begin("XSH.write.015");
	pc_check(pipe(p) == 0, "pipe failed");
	pc_check(close(p[0]) == 0, "close of the read end failed");
	memset(&sa, 0, sizeof sa);
	sa.sa_handler = on_pipe;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGPIPE, &sa, NULL) == 0, "sigaction failed");
	pc_test();
	pc_expect_err(write(p[1], "x", 1), EPIPE, "write() to a pipe with no reader");
	pc_check(got_pipe, "SIGPIPE was not sent");
	pc_pass();
}
