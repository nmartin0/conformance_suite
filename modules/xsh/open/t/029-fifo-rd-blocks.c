/*
 * Blocking read-only FIFO open waits for a writer; a caught signal gives EINTR.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.029
 * MUTANT    open.fifo_never_blocks
 * MUTANT    open.eintr_as_eagain
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

	pc_begin("XSH.open.029");
	pc_check(mkfifo("p", 0600) == 0, "mkfifo failed");
	arm_alarm(1);
	pc_test();
	fd = open("p", O_RDONLY);
	pc_check(fd == -1, "open() returned a descriptor although no writer exists");
	pc_expect_err(fd, EINTR, "blocked open(\"p\", O_RDONLY) interrupted by SIGALRM");
	pc_pass();
}
