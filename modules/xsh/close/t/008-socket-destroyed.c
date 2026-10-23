/*
 * Closing one end of a socket pair gives the other end end of file.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.close.008
 * MUTANT    close.noop
 */
#include "pc.h"
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>

static void on_alarm(int sig)
{
	(void)sig;
}

/* SIGALRM interrupts a blocked call: caught, without SA_RESTART. */
static void arm_alarm(unsigned secs)
{
	struct sigaction sa;

	sa.sa_handler = on_alarm;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	pc_check(sigaction(SIGALRM, &sa, NULL) == 0, "sigaction failed");
	alarm(secs);
}

int main(void)
{
	int sv[2];
	ssize_t n;
	char c;

	pc_begin("XSH.close.008");
	pc_check(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0, "socketpair failed");
	arm_alarm(5);
	pc_test();
	pc_expect_ok(close(sv[0]), "close(one end)");
	n = read(sv[1], &c, 1);
	if (n == -1 && errno == EINTR)
		pc_fail("read() at the other end still blocked after 5 s");
	pc_check(n == 0, "read() at the other end gave %ld, expected 0 (end of file)",
	    (long)n);
	pc_pass();
}
