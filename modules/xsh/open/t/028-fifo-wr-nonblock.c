/*
 * Non-blocking write-only open of a FIFO with no reader fails with ENXIO.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.028
 * MUTANT    open.nonblock_ignored
 * MUTANT    open.enxio_as_eagain
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
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
	int fd;

	pc_begin("XSH.open.028");
	pc_check(mkfifo("p", 0600) == 0, "mkfifo failed");
	arm_alarm(5);
	pc_test();
	fd = open("p", O_WRONLY | O_NONBLOCK);
	if (fd == -1 && errno == EINTR)
		pc_fail("open() blocked (interrupted after 5 s)");
	pc_expect_err(fd, ENXIO, "open(\"p\", O_WRONLY|O_NONBLOCK) with no reader");
	pc_pass();
}
