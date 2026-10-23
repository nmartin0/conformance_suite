/*
 * Blocking read-only FIFO open completes when a writer opens.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.open.030
 * MUTANT    open.fifo_block_enxio
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

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
	int fd, st;
	pid_t pid;

	pc_begin("XSH.open.030");
	pc_check(mkfifo("p", 0600) == 0, "mkfifo failed");
	pid = fork();
	pc_check(pid != -1, "fork failed");
	if (pid == 0) {
		/* The writer: blocks until the reader opens, then leaves. */
		alarm(10);
		_exit(open("p", O_WRONLY) >= 0 ? 0 : 1);
	}
	pc_test();
	arm_alarm(5);
	fd = open("p", O_RDONLY);
	if (fd == -1 && errno == EINTR)
		pc_fail("open() still blocked 5 s after a writer started");
	pc_expect_ok(fd, "open(\"p\", O_RDONLY) with a writer");
	pc_check(waitpid(pid, &st, 0) == pid, "waitpid failed");
	pc_pass();
}
