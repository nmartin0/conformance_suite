/*
 * tee -i ignores SIGINT: tee is run with the default SIGINT action
 * inherited, interrupted after it has copied some input, and must still
 * copy the rest and exit with status 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XCU.tee.004
 */
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "pc.h"

static void nap(long ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000L;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}

int main(void)
{
	int p[2];
	pid_t pid;
	int status, i;
	struct stat st;
	sigset_t none;

	pc_begin("XCU.tee.004");
	signal(SIGPIPE, SIG_IGN);
	pc_check(pipe(p) == 0, "pipe failed");
	pc_check(fflush(NULL) == 0, "fflush failed");
	pid = fork();
	pc_check(pid != -1, "fork failed");
	if (pid == 0) {
		int fd;
		struct sigaction sa;

		memset(&sa, 0, sizeof sa);
		sa.sa_handler = SIG_DFL;
		sigemptyset(&sa.sa_mask);
		sigaction(SIGINT, &sa, NULL);
		sigemptyset(&none);
		sigprocmask(SIG_SETMASK, &none, NULL);
		dup2(p[0], 0);
		close(p[0]);
		close(p[1]);
		fd = open("stdout.txt", O_WRONLY | O_CREAT | O_TRUNC, 0600);
		if (fd < 0)
			_exit(125);
		dup2(fd, 1);
		close(fd);
		execlp("tee", "tee", "-i", "out", (char *)NULL);
		_exit(127);
	}
	close(p[0]);
	pc_test();
	pc_check(write(p[1], "a\n", 2) == 2, "write to tee failed");
	/* wait until tee has copied the first bytes: it is running by then */
	for (i = 0; i < 300; i++) {
		if (stat("out", &st) == 0 && st.st_size >= 2)
			break;
		nap(100);
	}
	if (!(stat("out", &st) == 0 && st.st_size >= 2)) {
		kill(pid, SIGKILL);
		waitpid(pid, &status, 0);
		pc_unresolved("tee did not copy its first input within 30 seconds");
	}
	pc_check(kill(pid, SIGINT) == 0, "cannot signal tee");
	nap(300);
	pc_check(write(p[1], "b\n", 2) == 2, "tee ended after SIGINT despite -i");
	close(p[1]);
	for (i = 0; i < 300; i++) {
		pid_t r = waitpid(pid, &status, WNOHANG);

		if (r == pid)
			break;
		pc_check(r != -1, "waitpid failed");
		nap(100);
	}
	if (i == 300) {
		kill(pid, SIGKILL);
		waitpid(pid, &status, 0);
		pc_fail("tee did not end after its input was closed");
	}
	pc_check(!WIFSIGNALED(status), "tee was killed by a signal despite -i");
	pc_check(WIFEXITED(status) && WEXITSTATUS(status) == 0,
	    "tee exited with a nonzero status");
	pc_check(pc_file_equals("out", "a\nb\n"), "out does not hold both parts");
	pc_check(pc_file_equals("stdout.txt", "a\nb\n"),
	    "standard output does not hold both parts");
	pc_pass();
}
